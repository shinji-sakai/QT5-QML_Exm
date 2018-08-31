import QtQuick 2.9
import QtQuick.Controls 2.2

TextField {
    id: fromInput
    validator : RegExpValidator { regExp : /[0-9]+[0-9]/ }
    text: "0"
    onTextChanged: {
        while (text[0] === '0' && text.length != 1)
            text = text.slice(1)
        if (text == "") text = 0
    }
}
