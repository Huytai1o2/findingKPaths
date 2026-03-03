import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: root
    visibility: showMaximized()
    visible: true
    property alias image: image

    property bool showInfoPath: false
    property bool showReadMe: true
    property int currentPathIndex: 0
    property double pathDrawProgress: 0
    property double pathDrawStep: 0.15
    property alias canvas: canvas
    property alias fillComponent: fillComponent
    property alias scrollView: scrollView
    property alias infopaths: infopaths
    property alias infoText: infoText
    property alias textContainer: textContainer
    property alias destinationInput: destinationInput
    property var paths: []

    Column {
        id: khung
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Row {
            id: inputContainer
            width: parent.width
            height: parent.height * 0.05
            anchors.top: parent.top
            anchors.topMargin: 20

            Item {
                width: parent.width
                height: parent.height

                Row {
                    id: inputRow
                    width: parent.width * 0.8
                    height: 20
                    anchors.centerIn: parent

                    TextField {
                        id: sourceInput
                        width: parent.width / 5
                        placeholderText: "Enter the source"
                        horizontalAlignment: TextInput.AlignHCenter
                        anchors.right: destinationInput.left
                        validator: IntValidator {
                            bottom: 1
                            top: 69
                        }
                        anchors.rightMargin: 20
                    }

                    TextField {
                        id: destinationInput
                        width: parent.width / 5
                        placeholderText: "Enter the destination"
                        horizontalAlignment: TextInput.AlignHCenter
                        anchors.right: fillComponent.left

                        validator: IntValidator {
                            bottom: 1
                            top: 69
                        }
                        anchors.rightMargin: 10
                    }

                    Item {
                        id: fillComponent
                        anchors.centerIn: parent
                    }

                    TextField {
                        id: kInput
                        width: parent.width / 5
                        placeholderText: "Enter the number paths"
                        horizontalAlignment: TextInput.AlignHCenter
                        validator: IntValidator {
                            bottom: 0
                            top: 1000
                        }
                        anchors.left: fillComponent.right
                        anchors.leftMargin: 10
                    }

                    Button {
                        id: findPathButton
                        width: parent.width / 5
                        text: "Find the path"
                        onClicked: {
                            console.log("Source: " + sourceInput.text)
                            console.log("Destination: " + destinationInput.text)
                            console.log("k: " + kInput.text)

                            backend.sourceInput = sourceInput.text
                            backend.destinationInput = destinationInput.text
                            backend.kInput = kInput.text
                            backend.do_something()  // Call C++ method
                            showReadMe = false
                        }
                        anchors.left: kInput.right
                        anchors.leftMargin: 20
                    }
                }
            }
        }

        Row {
            id: contentContainer
            anchors.top: inputContainer.bottom
            anchors.bottom: khung.bottom
            anchors.right: khung.right
            anchors.left: khung.left

            Item {
                id: listAndmap
                width: parent.width
                anchors.top: parent.top
                height: parent.height

                Column {
                    id: textContainer
                    width: parent.width / 4
                    anchors.top: parent.top
                    height: parent.height
                    anchors.left: parent.left
                    anchors.leftMargin: 20

                    Label {
                        anchors.top: parent.top
                        anchors.topMargin: 83
                        anchors.left: parent.left
                        id: infoText
                        text: "<b>The information about the k shortest paths</b>"
                        textFormat: "RichText"
                        wrapMode: Text.WordWrap
                        font.pixelSize: root.height * 0.03
                        horizontalAlignment: Text.AlignHCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Column {
                        width: parent.width
                        anchors.bottom: textContainer.bottom
                        id : infopaths
                        anchors.top: infoText.bottom
                        anchors.topMargin: 10
                        anchors.left: parent.left

                        ScrollView {
                            id: scrollView
                            width: parent.width
                            height: parent.height * 0.9

                            Column {
                                width: parent.width

                                Text {
                                    visible: !showReadMe
                                    id: textContent
                                    width: textContainer.width
                                    text: backend.textOutput
                                    font.pixelSize: root.height * 0.02
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignJustify
                                }

                                Text {
                                    visible: showReadMe
                                    id: readMeInfo
                                    text: "Please type source ID from 1 to 69\n" +
                                                  "Please type destination ID from 1 to 69\n" +
                                                  "Please type k from 1 to 1000\n" +
                                                    "For more information, you can read in readMe.txt "
                                    font.pixelSize: root.height * 0.02
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignJustify
                                }
                            }
                        }
                    }

                    Text {
                        id: coppyRight
                        text: "Made by Huong vs Tai"
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 20
                        font.pixelSize: root.height * 0.015
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }

                Column {
                    id: mapContainer
                    spacing: 10
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.top: parent.top
                    anchors.rightMargin: 20
                    anchors.leftMargin: 20
                    anchors.left: textContainer.right

                    Flickable {
                        width: parent.width
                        height: parent.height * 0.8
                        anchors.bottom: pathInfo.top
                        anchors.right: mapContainer.right
                        anchors.bottomMargin: 20

                        contentWidth: Math.max(image.width * image.scale, root.width)
                        contentHeight: Math.max(image.height * image.scale, root.height)
                        clip: true

                        Image {
                            id: image

                            property real zoom: 0.0
                            property real zoomStep: 0.1

                            asynchronous: true
                            cache: false
                            smooth: true
                            antialiasing: true
                            mipmap: true

                            anchors.top: mapContainer.top
                            fillMode: Image.PreserveAspectFit
                            transformOrigin: Item.TopLeft
                            scale: Math.min(root.width / width, root.height / height, 1) + zoom

                            source: "qrc:/sketch1724773694325.png"

                            Canvas {
                                id: canvas
                                width: parent.width
                                height: parent.height
                                anchors.centerIn: parent

                                onPaint: {
                                    var ctx = canvas.getContext("2d");
                                    ctx.clearRect(0, 0, canvas.width, canvas.height);

                                    if (backend.paths.length > 0 && currentPathIndex < backend.paths.length) {
                                        var path = backend.paths[currentPathIndex];

                                        switch(currentPathIndex) {
                                            case 4: ctx.strokeStyle = "gray"; break;
                                            case 3: ctx.strokeStyle = "orange"; break;
                                            case 2: ctx.strokeStyle = "red"; break;
                                            case 1: ctx.strokeStyle = "blue"; break;
                                            case 0: ctx.strokeStyle = "green"; break;
                                            default: ctx.strokeStyle = "black"; break;
                                        }

                                        ctx.lineWidth = 10;

                                        if (path.length > 1) {
                                            ctx.beginPath();
                                            ctx.moveTo(path[0].x, path[0].y);

                                            for (var j = 1; j <  path.length; j++) {
                                                ctx.lineTo(path[j].x, path[j].y);
                                            }
                                            ctx.stroke();
                                        }
                                    }
                                }
                            }
                        }
                    }

                    MouseArea {
                        width: parent.width
                        height: parent.height * 0.8
                        anchors.centerIn: parent

                        acceptedButtons: Qt.NoButton

                        onWheel: {
                            if (wheel.angleDelta.y > 0)
                                image.zoom = Number((image.zoom + image.zoomStep).toFixed(1))
                            else
                                if (image.zoom > 0) image.zoom = Number((image.zoom - image.zoomStep).toFixed(1))

                            // Update the scale and content dimensions
                            image.scale = Math.min(root.width / image.width, root.height / image.height, 1) + image.zoom
                            flickable.contentWidth = image.width * image.scale
                            flickable.contentHeight = image.height * image.scale

                            wheel.accepted = true
                        }
                    }

                    Row {
                        id: navigationButtons
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottomMargin: 20

                        Button {
                            text: "The previous path"
                            width: 200
                            onClicked: {
                                if (currentPathIndex > 0) {
                                    currentPathIndex -= 1;
                                } else {
                                    currentPathIndex = backend.paths.length - 1;
                                }
                                pathDrawProgress = 0; // Reset the progress
                                canvas.requestPaint(); // Redraw the path
                            }
                        }

                        Button {
                            text: "The next path"
                            width: 200
                            onClicked: {
                                currentPathIndex = (currentPathIndex + 1) % backend.paths.length;
                                pathDrawProgress = 0; // Reset the progress
                                canvas.requestPaint(); // Redraw the path
                            }
                        }
                    }

                    Text {
                        id: pathInfo
                        visible: showInfoPath
                        text: "<b>This is the " + (currentPathIndex + 1) + getOrdinalSuffix(currentPathIndex + 1) + " path</b>"
                        textFormat: Text.RichText
                        font.pixelSize: root.height * 0.03
                        horizontalAlignment: Text.AlignHCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: navigationButtons.top
                        anchors.bottomMargin: 20
                    }
                }
            }
        }
    }

    function refreshCanvas() {
        currentPathIndex = 0;
        showInfoPath = true;
        pathDrawProgress = 0; // Reset the progress
        canvas.requestPaint(); // Redraw the path
    }

    function getOrdinalSuffix(number) {
        if (number % 10 === 1 && number % 100 !== 11) {
            return "st";
        } else if (number % 10 === 2 && number % 100 !== 12) {
            return "nd";
        } else if (number % 10 === 3 && number % 100 !== 13) {
            return "rd";
        } else {
            return "th";
        }
    }
}
