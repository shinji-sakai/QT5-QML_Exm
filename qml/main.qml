import QtQuick 2.9
import QtQuick.Window 2.2
import utility.config.yaml 1.0

Window {
    id: root
    visible: true
    width: 640
    height: 480

    ListModel {
        id: settingDir
        property int idx: -1

        onIdxChanged: inputWidget.refresh()
    }

    Component {
        id : configItem

        ConfigItem {}
    }

    ListView {
        id: yamlTreeViewer
        clip: true
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 10
        width: parent.width / 2
        anchors.right: inputWidget.left
        model: settingDir

        signal collapse(string category, bool collapsed)

        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            width: parent.width

            border.color: "gray"
            color: "transparent"
        }

        delegate: configItem
    }

    ConfigManager {
        id: cfgManager
        function setUp() {
            var categoryList = categories();
            var keyList = keys();
            var list = [];

            categoryList.forEach(function (category) {
                list.push({ type : "category", name : category });
            })

            for (var idx = keyList.length - 1; idx >= 0; idx --) {
                var parentKey = keyList[idx].split('.')[0];

                var parentIdx = list.indexOf(list.find(function (elem) {
                    if (elem.type === "category" && elem.name === parentKey)
                        return true;
                    return false;
                }))

                if (parentIdx >= 0)
                    list.splice(parentIdx + 1, 0, { type : "key", name : keyList[idx] });
            }

            settingDir.clear();
            list.forEach(function (item) {
                settingDir.append(item);
            })
        }
        language: "en_US"

        Component.onCompleted: {
            loadConfig("config.yaml");
            setUp();
        }
    }

    InputWidget {
        id: inputWidget
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: parent.width / 2 - 40
        anchors.margins: 20

        Rectangle {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            width: parent.width
            anchors.margins: -10
            color: "transparent"
            border.color: "gray"
        }
    }

    Timer {
        interval: 5000
        running: true
        onTriggered: cfgManager.saveConfig("D:\\a.yaml")
    }
}
