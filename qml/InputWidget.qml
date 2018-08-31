import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQml.Models 2.3
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.0
import QtQuick.Window 2.3

Item {

    Column {
        anchors.fill: parent
        anchors.leftMargin: 20

        Label {
            visible: false
            id: description
            text: "Description here"
            font.pointSize: 14
            bottomPadding: 10
        }

        Switch {
            id: boolInput
            visible: false
            onCheckedChanged: {
                var name = settingDir.get(settingDir.idx).name;
                cfgManager.setKeyValue(name, checked);
            }

            function initDefault(value) {
                visible = true;
                boolInput.checked = value.default;
            }
        }

        TextField {
            id: stringInput
            visible: false
            onTextChanged: {
                var name = settingDir.get(settingDir.idx).name;
                cfgManager.setKeyValue(name, text);
            }

            function initDefault(value) {
                visible = true;
                stringInput.text = value.default;
            }
        }

        Column {
            id: intInput
            width: parent.width
            visible: false

            Row {
                spacing: 10

                Label {
                    anchors.verticalCenter: fromInput.verticalCenter
                    text: "Min:"
                }

                Label {
                    id: fromInput
                    color: "green"
                }

                Label {
                    anchors.verticalCenter: toInput.verticalCenter
                    text: "Max:"
                }

                Label {
                    id: toInput
                    color: "green"
                }
            }

            Row {
                width: parent.width
                spacing: 10

                Slider {
                    id: defaultInput
                    width: parent.width - defaultValue.width - 10
                    from: parseInt(fromInput.text) ? parseInt(fromInput.text) : 0
                    to: parseInt(toInput.text) ? parseInt(toInput.text) : 0
                    stepSize: 1
                    onValueChanged: {
                        var name = settingDir.get(settingDir.idx).name;
                        cfgManager.setKeyValue(name, value);
                    }
                }

                Label {
                    id: defaultValue
                    anchors.verticalCenter: defaultInput.verticalCenter
                    width: 50
                    horizontalAlignment: Qt.AlignRight
                    text: defaultInput.value ? defaultInput.value : 0
                }
            }

            function initDefault(value) {
                var defValue = value.default;
                visible = true;
                toInput.text = value.max ? value.max : 0
                fromInput.text = value.min ? value.min : 0
                defaultInput.value = defValue ? defValue : 0
            }
        }

        ComboBox {
            id: enumInput
            visible: false
            implicitHeight: 40

            onCurrentIndexChanged: {
                var name = settingDir.get(settingDir.idx).name;
                cfgManager.setKeyValue(name, currentIndex);
            }

            function initDefault(value) {
                var defValue = value.default;
                visible = true;
                enumInput.model = value.entries;
                enumInput.currentIndex = defValue;
            }
        }

        Column {
            id: intListInput
            width: parent.width
            visible: false

            Row {
                spacing: 10

                Label {
                    anchors.verticalCenter: listFromInput.verticalCenter
                    text: "Min:"
                }

                Label {
                    id: listFromInput
                    color: "green"
                }

                Label {
                    anchors.verticalCenter: listToInput.verticalCenter
                    text: "Max:"
                }

                Label {
                    id: listToInput
                    color: "green"
                }
            }

            Column {
                Repeater {
                    id: listInts
                    Row {
                        width: parent.width
                        spacing: 10

                        Slider {
                            id: defaultListInput
                            width: parent.width - defaultListValue.width - 10
                            from: parseInt(listFromInput.text) ? parseInt(listFromInput.text) : 0
                            to: parseInt(listToInput.text) ? parseInt(listToInput.text) : 0
                            stepSize: 1
                            onValueChanged: intListInput.report();
                        }

                        Label {
                            id: defaultListValue
                            anchors.verticalCenter: defaultInput.verticalCenter
                            width: 50
                            horizontalAlignment: Qt.AlignRight
                            text: defaultInput.value ? defaultInput.value : 0
                        }
                        function getDefault() {
                            return defaultInput.value;
                        }
                    }
                }
            }

            function report() {
                var name = settingDir.get(settingDir.idx).name;
                var res = [];
                for (var idx = 0; idx < listInts.count; idx ++)
                    res.push(listInts.itemAt(idx).getDefault());
                cfgManager.setKeyValue(name, res);
            }

            function initDefault(value) {
                visible = true;
                toInput.text = value.max ? value.max : 0
                fromInput.text = value.min ? value.min : 0
                value.default.forEach(function (def) {
                    defaultInput.value = value.default ? value.default : 0;
                })
            }
        }

        Column {
            id: enumListWrapper
            width: parent.width
            height: 300

            Column {
                Repeater {
                    id: enumListRepeater
                    ComboBox {
                        id: enumListInput
                        implicitHeight: 40

                        onCurrentIndexChanged: enumListWrapper.report();

                        Component.onCompleted: {
                            enumListWrapper.setModel.connect(setModel);
                            enumListWrapper.setDefault.connect(setDefault);
                        }

                        function setModel(newModel) {
                            model = newModel;
                        }

                        function setDefault(idx, defValue) {
                            if (index === idx)
                                currentIndex = defValue;
                        }

                        function getDefault() {
                            return currentIndex;
                        }
                    }
                }
            }

            signal setModel(var model);
            signal setDefault(int idx, int defValue);

            function report() {
                var name = settingDir.get(settingDir.idx).name;
                var res = [];
                for (var idx = 0; idx < enumListRepeater.count; idx ++)
                    res.push(enumListRepeater.itemAt(idx).getDefault());
                cfgManager.setKeyValue(name, res);
            }

            function initDefault(value) {
                visible = true;
                var defaultList = value.default;
                enumListRepeater.model = defaultList.length;
                setModel(value.entries);
                defaultList.forEach(function (defValue, idx) {
                    setDefault(idx, defValue);
                })
            }
        }
    }

    function refresh() {
        var obj = settingDir.get(settingDir.idx);
        description.visible = true;

        boolInput.visible = false;
        stringInput.visible = false;
        intInput.visible = false;
        enumInput.visible = false;
        intListInput.visible = false;
        enumListWrapper.visible = false;

        description.text = cfgManager.getDescription(obj.name);
        description.visible = description.text !== "";

        var value = cfgManager.getKeyValue(obj.name);

        if (value.objType === "key") {
            switch (value.type) {
            case "bool":
                boolInput.initDefault(value);
                break;
            case "string":
                stringInput.initDefault(value);
                break;
            case "int":
                intInput.initDefault(value);
                break;
            case "enum":
                enumInput.initDefault(value);
                break;
            case "list<int>":
                intListInput.initDefault(value);
                break;
            case "list<enum>":
                enumListWrapper.initDefault(value);
                break;
            }
        }
    }
}
