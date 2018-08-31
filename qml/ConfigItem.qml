import QtQuick 2.9
import QtQuick.Controls 2.2

Item {
    id: wrapper
    height: line.visible ? 20 : 0
    width: parent.width

    MouseArea {
        anchors.fill: line
        hoverEnabled: true
        Rectangle {
            anchors.fill: parent
            color: "green"
            opacity: settingDir.idx === index ? 0.5 : parent.containsMouse ? 0.1 : 0.0
        }

        onClicked: settingDir.idx = index
    }

    Row {
        id: line
        anchors.fill: parent
        leftPadding: type === "category" ? 5 : 35

        Behavior {
            NumberAnimation {
                properties: "x, opacity"
                duration: 200
            }
        }

        ToolButton {
            id: dropper
            width: 20
            height: 20
            visible: type === "category"
            checked: true
            checkable: true
            anchors.verticalCenter: parent.verticalCenter

            contentItem: Image {
                source: "qrc:/images/dropper.png"
                sourceSize.width: 20
                sourceSize.height: 20
                fillMode: Image.Stretch
                rotation: parent.checked ? 90 : 0
                Behavior on rotation {
                    NumberAnimation { duration: 200 }
                }
            }

            onClicked: yamlTreeViewer.collapse(name, !checked)
        }

        Label {
            anchors.verticalCenter: dropper.verticalCenter
            text: nameTheItem()

            function nameTheItem() {
                return cfgManager.getName(name);
            }

            Component.onCompleted : {
                nameTheItem();
                cfgManager.languageChanged.connect(nameTheItem);
            }
        }

        function collapse(category, collapsed) {
            if (type === "key" && typeof name === "string" && name.split(".")[0] === category)
                visible = !collapsed;
        }

        Component.onCompleted: yamlTreeViewer.collapse.connect(collapse)
    }
}
