Program test;

var
    x: integer;

function sum(x: integer; y: integer): integer;
begin
    { sum := sum(sum(3, 2), 1) + true; }
    x := 2;
    { sum := true <> false = true >= true; }

    sum := 3;
end;

begin
    if 2 < sum(3, 2) then
        begin
        x := 4;
        end
    else
        begin

        x := 'a';
        end;

    x := sum(5, 10);  { Example call to the function }
    writeln('The sum is: ', x);
end.