import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import Shader 1.0
Item {
    id:root
    width: 800
    height: 600


    Shader {
        id:shader
        width: 400
        height:400
        x:200
        y:100
//        SequentialAnimation on x {
//            loops: Animation.Infinite
//            PropertyAnimation { to: 300 }
//            PropertyAnimation { to: 200 }
//            running:true
//        }
        states: [
            State{
                name:"TopLeft"
                PropertyChanges  {target:shader; x:0; y:0;}
            },
            State{
                name:"TopRight"
                PropertyChanges  {target:shader; x:400; y:0;}
            },
            State{
                name:"BottomLeft"
                PropertyChanges  {target:shader; x:0; y:200;}
            },
            State{
                name:"BottomRight"
                PropertyChanges  {target:shader; x:400; y:200;}
            },
            State{
                name:"Middle"
                PropertyChanges  {target:shader; x:200; y:100;}
            }
        ]
        transitions: Transition {
                  PropertyAnimation { duration: 750; properties: "x,y"; easing.type: Easing.InOutQuad }
        }
        state:"Middle"
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

}
