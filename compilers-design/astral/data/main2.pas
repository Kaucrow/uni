Program test;

var
    x: integer;

function sum(x: integer; y: integer): boolean;
begin
    { sum := sum(sum(3, 2), 1) + true; }
    x := 2;
    sum := true <> false = true >= true;
end;

begin
    if 2 < 4 then
        begin
        x := 4;
        end
    else
        begin

        x := 2;
        end;

    x := sum(5, 10);  { Example call to the function }
    writeln('The sum is: ', x);
end.