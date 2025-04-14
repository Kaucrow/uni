var x = 32;

@component MyCheckbox
    style {
        p {
            color: red;
            font-weight: bold;
        }

        input {
            cursor: pointer;
        }
    }

    template {
        <input type="checkbox">
    }

    var check = false
    var other = true

    bind check to input.checked

    on input change {
        if (this.check) {
            console.log('The feature was enabled');
        } else {
            console.log('The feature was disabled');
        }
    }
@end

var y = 42;