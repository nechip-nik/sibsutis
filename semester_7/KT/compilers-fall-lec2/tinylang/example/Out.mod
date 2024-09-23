MODULE Out;

IMPORT Terminal;

PROCEDURE Write(s: ARRAY OF CHAR);
VAR i: INTEGER;
BEGIN
  i := 0;
  WHILE s[i] # 0C DO
    Terminal.Write(s[i]);
    INC(i);
  END;
END Write;

PROCEDURE WriteInt(n: INTEGER; width: INTEGER);
VAR s: ARRAY 16 OF CHAR;
    i, j, k: INTEGER;
BEGIN
  i := 0;
  IF n < 0 THEN
    Terminal.Write('-');
    n := -n;
  END;
  REPEAT
    s[i] := CHR(n MOD 10 + ORD('0'));
    n := n DIV 10;
    INC(i);
  UNTIL n = 0;
  FOR j := 1 TO width-i DO
    Terminal.Write(' ');
  END;
  FOR k := i-1 TO 0 BY -1 DO
    Terminal.Write(s[k]);
  END;
END WriteInt;

PROCEDURE WriteLn;
BEGIN
  Terminal.WriteLn;
END WriteLn;

END Out.