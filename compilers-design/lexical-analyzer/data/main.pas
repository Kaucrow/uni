Program test;

var
    x: integer;

function sum(x: integer; y: integer): integer;
begin
    sum := x + y;
end;

begin
    x := sum(5, 10);  { Example call to the function }
    writeln('The sum is: ', x);
end.