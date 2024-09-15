% Предикат, который переставляет строки в обратном порядке
reverse_lines(InputFile, OutputFile) :-
    open(InputFile, read, ReadStream),
    read_lines(ReadStream, Lines),
    close(ReadStream),
    reverse(Lines, ReversedLines),
    open(OutputFile, write, WriteStream),
    write_lines(WriteStream, ReversedLines),
    close(WriteStream).

% Предикат, который читает строки из файла и возвращает список строк
read_lines(Stream, []) :-
    at_end_of_stream(Stream).
read_lines(Stream, [Line | Rest]) :-
    \+ at_end_of_stream(Stream),
    read_line_to_string(Stream, Line),
    read_lines(Stream, Rest).

% Предикат, который записывает список строк в файл
write_lines(_, []).
write_lines(Stream, [Line | Rest]) :-
    write(Stream, Line),
    nl(Stream),
    write_lines(Stream, Rest).