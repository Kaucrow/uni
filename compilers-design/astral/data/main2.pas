Program test;

var
    x: integer;

function sum(x: integer; y: integer): integer;
begin
    var
        z: integer;

    sum := (x + z) * 4.3 - sum(sum(3, 5), 1);
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