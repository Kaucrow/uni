var x = 32;

@component MyCheckbox
    style {
        p {
            color: red;
            font-weight: bold;
        }
    }

    template {
        <input type="checkbox">
        <p>Checkbox 1</p>
    }

    var check = false
    var other = true

    bind check to input.checked
@end

var y = 42;