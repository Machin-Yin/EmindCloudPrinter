import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import com.client.emindprint 1.0
import "client.js" as Jsclient


Window {
    objectName: "clientWin"
    id:clientWin
    width: 400
    height: 240
    title: qsTr("EmindCloudPrinter")
    maximumHeight: 240
    minimumHeight: 240
    maximumWidth: 400
    minimumWidth: 400
    visible:true


    property int reqok:0
    property int reqAuth:0

    signal showPrinterWin()


    PrinterList{
        id:pWin
        signal showList()
        signal stopSpinner()
    }

    Connections{
        target:clientWin
        onShowPrinterWin:{
            pWin.visible = true;

        }
    }


    ClientAssistant{
        id:clientAssist
        onSigConnected: {
            clientWin.showPrinterWin();
            clientWin.hide();
        }
        onSigAuthWrong:{
            errText2.color="red";
            errText2.visible = true;
            fieldLicense.style = fieldStyleErr;
        }
        onSigConnectRefused: {
            errText1.visible = true;
            errText1.color="red";
            fieldIP.style=fieldStyleErr;
        }
        onPlistSent:{
//            Jsclient.g_str = client.pnameStr;
            Jsclient.g_str = clientAssist.pnameStr;
            console.log("ClientLogin.qml:",Jsclient.g_str);
//            console.log(Jsclient.g_str);
            pWin.showList();
        }
        onPrinterSetFinished:{
            pWin.stopSpinner();
        }
//        onStopIndicator:{
//            pWin.stopSpinner();
//        }
    }


//    EmindClient{
//        id:client
//        onRcvCupsFile:clientWin.visible = false;
//        onSigConnected: {
//            clientWin.showPrinterWin();
//            clientWin.hide();
//        }
//        onSigAuthWrong:{
//            errText2.color="red";
//            errText2.visible = true;
//            fieldLicense.style = fieldStyleErr;
//        }
//        onSigConnectRefused: {
//            errText1.visible = true;
//            errText1.color="red";
//            fieldIP.style=fieldStyleErr;
//        }
//        onPlistSent:{
//            Jsclient.g_str = client.pnameStr;
//            console.log(Jsclient.g_str);
//            pWin.showList();
//        }
//        onPrinterSetFinished:{
//            pWin.stopSpinner();
//        }
//        onStopIndicator:{
//            console.log("stop indicator");
//            pWin.stopSpinner();
//        }

//    }

//    Connections{
//        target:clientAssist
//        onSigConnected: {
//            clientWin.showPrinterWin();
//            clientWin.hide();
//        }
//        onSigAuthWrong:{
//            errText2.color="red";
//            errText2.visible = true;
//            fieldLicense.style = fieldStyleErr;
//        }
//        onSigConnectRefused: {
//            errText1.visible = true;
//            errText1.color="red";
//            fieldIP.style=fieldStyleErr;
//        }
//    }


    Component {
        id:fieldStyleErr
        TextFieldStyle{
            background: Rectangle{
                border.color: "red";
            }
        }
    }


    Button {
        objectName:"btnCancel"
        id: btnCancel1
        x: 88
        y: 173
        width: 103
        height: 36
        style: ButtonStyle {
            background: Rectangle{
                radius: 5
                border.color: "black"
                color: (control.hovered || control.focus)? "blue":"white"
            }
        }

        text: qsTr("Cancel")

        onClicked:
        {
            Qt.quit();
        }
        Keys.onPressed: {
            if(event.key === Qt.Key_Enter)
                btnCancel1.clicked();
        }
        activeFocusOnPress:true
    }



    Button {
        objectName:"btnNext"
        id: btnNext
        x: 210
        y: 173
        width: 103
        height: 36
//        activeFocusOnPress:true
        KeyNavigation.tab: btnCancel1


        style: ButtonStyle {
            background: Rectangle{
                radius: 5
                border.color: "black"
                color: (control.hovered || control.focus)? "blue":"white"
            }
        }

        Keys.onPressed: {
            if(event.key === Qt.Key_Enter)
                btnNext.clicked();
        }

        text: qsTr("Next")

        enabled: enableBtnNext(btnNext.Text.text)


        onClicked:
        {
            btnNext.enabled = false;

            clientAssist.checkConnectivity(fieldIP.text,fieldLicense.text);
            btnNext.enabled = true;
        }
    }

    function enableBtnNext(){
        if((fieldIP.text !== "") && (fieldLicense.text !== "")){
            return true;
        }else{
            return false;
        }
    }

    Text {
        id: text1
        x: 62
        y: 49
        width: 119
        height: 29
        opacity: 0.9
        font.pixelSize: 14
        text:qsTr("Server Address:")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
    }

    Text {
        id: text2
        x: 65
        y: 113
        width: 119
        height: 30
        font.pixelSize: 14
        text:qsTr("  License:")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
    }

    Text {
        objectName: "errText1"
        id: errText1
        x: 158
        y: 79
        width: 182
        height: 28
        text: qsTr("Connect failed")
        visible: false
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        //        wrapMode: Text.WordWrap
        font.pixelSize: 14

    }

    Text {
        objectName: "errText2"
        id: errText2
        x: 158
        y: 142
        width: 182
        height: 31
        text: qsTr("License error")
        visible: false
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 14
    }

    TextField {
        id: fieldIP
        x: 202
        y: 50
        width: 121
        height: 29
        font.pixelSize: 12
        placeholderText: qsTr("")
        focus: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        //        inputMask: "\\  \\  \\  .\\ \\ \\ .\\ \\ \\ .\\ \\ \\ ;_"
    }

    TextField {
        id: fieldLicense
        x: 203
        y: 112
        width: 120
        height: 30
        font.pixelSize: 12
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        placeholderText: qsTr("")
        KeyNavigation.tab: btnNext

    }



}
