import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import com.client.emindprint 1.0
import QtQuick.Layouts 1.1
import "client.js" as Jsclient

Window {
    objectName: "printerWin"
    id:printerlist
    maximumHeight: 500
    minimumHeight: 500
    maximumWidth: 600
    minimumWidth: 600
    visible:false

    title:qsTr("Add Remote Printers")
    property string pnameStr1: ""
    property var pNameList:[]

//    signal setDefaultPrinter(var text,var idx)

//    signal printerAdded(string prName)
//    property string printerName

//    PrinterListModel{
//        id:pModel
//    }
//    EmindClient{
//        id:client
//    }

    Connections{
        target: printerlist
        onShowList:{
            var pname = new Array;
            pname= Jsclient.g_str.split(',');
            console.log(pname);
            for(var i=0;i<pname.length;i++)
                pModel.append({"prname":pname[i]});
        }

    }

    ListModel{
        id:pModel
    }

    ListView{
        id:pView
        anchors.fill:parent
        model:pModel
        delegate:pDelegate
        anchors.margins: 15
//        highlight: highlighter
//        highlightFollowsCurrentItem: true
//        focus:true
        Layout.alignment: Qt.AlignCenter
//        z:-1
    }


    Component{
        id:highlighter
        Rectangle{
            z:1
            opacity: 0.8
            width: pView.width
            height:60
            radius: 5
//            MouseArea{
//                hoverEnabled: true
//                anchors.fill: parent
//                onHoveredChanged: {
//                    parent.color = "#f5f5f5";
//                }
//            }
            color:"#f5f5f5"
        }
    }




    Component{
        id:pDelegate

        Rectangle{
            id:printerItem
            width:parent.width
            height:60

            Image{
                id:pImg
                height: 40
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                source:"img/printer.png"
            }

            Text{
                id:printerName
                text:prname
                font.pixelSize: 18
                anchors.left: parent.left
                anchors.leftMargin: pImg.width
                anchors.verticalCenter: parent.verticalCenter
            }

            Component{
                id:btnStyle1
                ButtonStyle{
                    background: Rectangle{
                        width:control.width
                        height:control.height
                        color:"#f5f5f5"
                    }
                    label:Text{
                        color:control.hovered?"blue":"black"
//                        text:busyIndicator.running ? qsTr("") : qsTr("Add")
                        text:qsTr("Add")
                        font.pixelSize: 18
                        anchors.fill: parent

                    }
                }
            }

            Component{
                id:btnStyle2
                ButtonStyle{
                    background: Rectangle{
                        width:control.width
                        height:control.height
                        color:"white"
                    }
                    label:Text{
                        color:control.hovered?"blue":"black"
//                        text:busyIndicator.running ? qsTr("") : qsTr("Add")
                        text:qsTr("Add")
                        font.pixelSize: 18
                        anchors.fill: parent

                    }
                }
            }


            MouseArea{
                hoverEnabled: true
                anchors.fill: parent
                onHoveredChanged: {

                    pView.currentIndex = index;
                }
                onEntered: {

                    printerItem.color = "#f5f5f5";
                    btnAdd.style = btnStyle1;
                }
                onExited: {
                    btnAdd.style = btnStyle2;
                    printerItem.color = "white";
                }
            }



            Button{
                id:btnAdd
                width:40
                height: 40
                anchors.right: printerItem.right
                anchors.verticalCenter: parent.verticalCenter
//                anchors.margins:20
                style:ButtonStyle{
                    background: Rectangle{
                        width:control.width
                        height:control.height
                    }
                    label:Text{
                        id:btnText
                        color:control.hovered?"blue":"black"
//                        text:busyIndicator.running ? qsTr("") : qsTr("Add")
                        text:qsTr("Add")
                        font.pixelSize: 18
                        anchors.fill: parent
//                        anchors.verticalCenter: btnAdd.verticalCenter
//                        onHoveredLinkChanged:  color="blue"
                    }
                }
                onClicked: {
//                    busyIndicator.visible = true;
//                    busyIndicator.running = true;
//                    console.log("index=",index);
                      clientAssist.setDefaultPrinter(printerName.text,index);
//                    busyIndicator.visible = false;
//                    busyIndicator.running = false;
//                    btnAdd.enabled = false;
                }

                BusyIndicator {
                    id:busyIndicator
                    anchors.fill: parent
                    width:45
                    height: 45
                    visible: false
                    running: false
                    Timer{
                        id:timer
                        interval: 1000
                        running: true
                        repeat: true
                        onTriggered:
                        {
                            busyIndicator.visible = false;
                            busyIndicator.running = false;
                        }
                    }

                    Connections{
                        target: printerlist
                        onStopSpinner:{
                            console.log("stop busyIndicator");
                            timer.start();

                        }
                    }


                }
            }

        }
    }


}



