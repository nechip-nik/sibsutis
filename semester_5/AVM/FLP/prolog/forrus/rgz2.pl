% Предикат для удаления слов максимальной длины из строки
delete_max_length_words([], []).
delete_max_length_words(String, NewString) :-
    split_string(String, " ", "", Words), % Разбиваем строку на слова
    find_max_word_length(Words, MaxLength),
    delete_words(Words, MaxLength, [], NewWords), % Удаляем слова максимальной длины
    atomic_list_concat(NewWords, " ", NewString). % Соединяем слова обратно в строку

% Предикат для поиска максимальной длины слова в списке
find_max_word_length([], 0).
find_max_word_length([Word|Words], MaxLength) :-
    atom_length(Word, Length),
    find_max_word_length(Words, MaxLength1),
    MaxLength is max(Length, MaxLength1).

% Предикат для удаления слов максимальной длины из списка
delete_words([], _, Acc, Acc).
delete_words([Word|Words], MaxLength, Acc, NewWords) :-
    atom_length(Word, Length),
    (Length = MaxLength -> delete_words(Words, MaxLength, Acc, NewWords) ; 
    delete_words(Words, MaxLength, [Word|Acc], NewWords)).

% Предикат для обработки каждой строки в файле
process_file(InputFile, OutputFile) :-
    open(InputFile, read, InputStream),
    open(OutputFile, write, OutputStream),
    process_lines(InputStream, OutputStream),
    close(InputStream),
    close(OutputStream).

% Предикат для обработки каждой строки из входного потока и записи новых строк в выходной поток
process_lines(InputStream, OutputStream) :-
    repeat,
    read_line_to_codes(InputStream, String), % Считываем строку из файла
    (String = end_of_file -> true ; % Если достигнут конец файла, завершаем предикат
    string_codes(Line, String), % Преобразуем строку из байтового кода в обычную строку
    delete_max_length_words(Line, NewLine), % Удаляем слова максимальной длины из строки
    format(OutputStream, '~w~n', [NewLine]), % Записываем новую строку в файл
    fail). % Зацикливаем предикат, чтобы он обработал все строки из файла
