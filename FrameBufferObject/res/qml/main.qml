import QtQuick 2.5
import QtQuick.Controls 1.4 as QQ1
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import Shader 1.0
import QtQuick.Window 2.2

Item {
    id:root
    width: Screen.width / 2
    height: Screen.height / 2
    Shader {
        id:shader
        width: 400
        height:400
        x:(root.width - width) / 2
        y:(root.height - height) / 2
        xRotate:0
        yRotate:0
        zRotate:0
        full:false
        states: [
            State{
                name:"TopLeft"
                PropertyChanges  {target:shader; x:0; y:0;}
            },
            State{
                name:"TopRight"
                PropertyChanges  {target:shader; x:root.width - width; y:0;}
            },
            State{
                name:"BottomLeft"
                PropertyChanges  {target:shader; x:0; y:root.height - height;}
            },
            State{
                name:"BottomRight"
                PropertyChanges  {target:shader; x:root.width - width; y:root.height - height;}
            },
            State{
                name:"Middle"
                PropertyChanges  {target:shader; x:(root.width - width)/2; y:(root.height - height) / 2;}
            }
        ]
        transitions: Transition {
            PropertyAnimation { duration: 750; properties: "x,y"; easing.type: Easing.InOutQuad }
        }
        state:"Middle"
        Text {
            text:shader.fps.toFixed(2)
            color:"green"
            font.pixelSize: 12
            anchors.left: parent.left
            anchors.top:parent.top
        }
    }
    Row {
        anchors.top:parent.top;
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 5
        Button {
            text:"FullScreen"
            onClicked: shader.full = !shader.full
        }
        Button {
            text:"Quit"
            onClicked: Qt.quit()
        }
    }
    Button {
        text:"TopLeft"
        anchors{top:parent.top; left:parent.left}
        onClicked: shader.state = "TopLeft"
    }
    Button {
        text:"TopRight"
        anchors{top:parent.top; right:parent.right}
        onClicked: shader.state = "TopRight"
    }
    Button {
        text:"BottomLeft"
        anchors{bottom:parent.bottom; left:parent.left}
        onClicked: shader.state = "BottomLeft"
    }
    Button {
        text:"BottomRight"
        anchors{bottom:parent.bottom; right:parent.right}
        onClicked: shader.state = "BottomRight"
    }
    Button {
        text:"middle"
        anchors.centerIn: parent
        onClicked: shader.state = "Middle"
    }
    Column {

        anchors{
            left:parent.left
            verticalCenter: parent.verticalCenter
        }
        Row{
            Text{
                text:"X"
                anchors.verticalCenter: parent.verticalCenter
            }
            QQ1.Slider {
                value:0
                minimumValue:0
                maximumValue :360
                onValueChanged: {
                    shader.xRotate = value;
                }
            }
        }
        Row{
            Text{
                text:"Y"
                anchors.verticalCenter: parent.verticalCenter
            }
            QQ1.Slider {
                value:0
                minimumValue:0
                maximumValue :360
                onValueChanged:{
                    shader.yRotate = value;
                }
            }
        }
        Row{
            Text{
                text:"Z"
                anchors.verticalCenter: parent.verticalCenter
            }
            QQ1.Slider{
                value:0
                minimumValue:0
                maximumValue :360
                onValueChanged: {
                    shader.zRotate = value;
                }
            }
        }
    }
}
