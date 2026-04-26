import cv2
import os
import sys
import warnings

warnings.filterwarnings("ignore")
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

from deepface import DeepFace

DATASET_PATH = "dataset"
os.makedirs(DATASET_PATH, exist_ok=True)

print("[INFO] Scanning face database...")


face_database = []
if os.path.exists(DATASET_PATH):
    for person_name in os.listdir(DATASET_PATH):
        person_dir = os.path.join(DATASET_PATH, person_name)
        if os.path.isdir(person_dir):
            for img_file in os.listdir(person_dir):
                if img_file.lower().endswith((".jpg", ".png", ".jpeg")):
                    face_database.append(os.path.join(person_dir, img_file))

print(f"[INFO] Memorized {len(face_database)} photos. Starting camera...")
print("[CONTROLS] 'q' - quit, 's' - save current face to database")

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

frame_skip = 0
last_label = "Searching for face..."
last_color = (255, 0, 0)

while True:
    ret, frame = cap.read()
    if not ret:
        break
    
    display_frame = frame.copy()
    frame_skip += 1
    
    # Analyze every 10th frame
    if frame_skip % 10 == 0:
        try:
            if len(face_database) > 0:
                dfs = DeepFace.find(
                    img_path=frame, 
                    db_path=DATASET_PATH, 
                    model_name="Facenet",
                    detector_backend="opencv",
                    enforce_detection=False,
                    silent=True
                )
                
                if dfs and len(dfs[0]) > 0:
                    best_match = dfs[0].iloc[0]
                    identity = os.path.basename(os.path.dirname(best_match['identity']))
                    distance = best_match['distance']
                    
                    if distance < 0.4:
                        last_label = f"{identity} ({distance:.2f})"
                        last_color = (0, 255, 0)  # Green - recognized
                    else:
                        last_label = "Face not found"
                        last_color = (0, 0, 255)  # Red - not recognized
                else:
                    last_label = "Face not found"
                    last_color = (0, 0, 255)  # Red - no face in database match
            else:
                last_label = "Database empty. Press 's' to add face"
                last_color = (0, 255, 255)  # Yellow
                
        except Exception as e:
            last_label = "Face not found"
            last_color = (0, 0, 255)
            # Print error only once per 50 frames for debugging
            if frame_skip % 50 == 0:
                print(f"[DEBUG] {str(e)[:50]}")
    
    # Display result
    cv2.putText(display_frame, last_label, (20, 50), 
                cv2.FONT_HERSHEY_SIMPLEX, 1, last_color, 2)
    cv2.putText(display_frame, "Q-quit | S-save", (20, 450), 
                cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)
    cv2.imshow('Face Recognition', display_frame)
    
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
    elif key == ord('s'):
        # Save current frame as new face
        name = input("Enter person's name (in terminal): ")
        if name:
            save_dir = os.path.join(DATASET_PATH, name)
            os.makedirs(save_dir, exist_ok=True)
            filename = f"{save_dir}/captured_{len(os.listdir(save_dir))}.jpg"
            cv2.imwrite(filename, frame)
            face_database.append(filename)
            print(f"[+] Face saved: {filename}")

cap.release()
cv2.destroyAllWindows()