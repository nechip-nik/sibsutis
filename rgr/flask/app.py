from flask import Flask, request, render_template_string
import mysql.connector

app = Flask(__name__)

HTML = """
<h1>вм2 </h1>
<h1>Сервис заказов книг</h1>

<form method="POST">
    <input name="title" placeholder="Название книги">
    <button type="submit">Заказать</button>
</form>

<h2>Все заказы:</h2>
<ul>
{% for row in rows %}
<li>{{ row[1] }} — {{ row[2] }}</li>
{% endfor %}
</ul>
"""

def db():
    return mysql.connector.connect(
        host="mariadb",
        user="appuser",
        password="apppass",
        database="books"
    )

@app.route("/", methods=["GET", "POST"])
def index():
    conn = db()
    cur = conn.cursor()

    if request.method == "POST":
        title = request.form.get("title")
        cur.execute("INSERT INTO orders(title, status) VALUES (%s, %s)", (title, "принят"))
        conn.commit()

    cur.execute("SELECT * FROM orders")
    rows = cur.fetchall()

    return render_template_string(HTML, rows=rows)

if __name__ == "__main__":
    app.run(host="0.0.0.0")
