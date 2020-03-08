//Variables
var url_socket = "ws://192.168.4.1/ws";
var button;
var canvas;
var context;
var flow = 0;
var pump = 0;
var conn;

// Called whenever the HTML button is pressed
function changeOutput() {
	onPress();
	counter();
}

function counter()
{
	var x = document.getElementById("myCheck").checked;
	var y = document.getElementById("myRange2").value;
	if (x==true) 
	{		  
		var counter = y;	
		setInterval(function()
		{
			counter--;
			x = document.getElementById("myCheck").checked;				
			if (counter > 0 & x==true)
			{
				id = document.getElementById("count");
				id.innerHTML=counter;
			}
			else if (counter == 0 & x==true)
			{
				id.innerHTML="COMPLETE";
			}
			else if (x==false) 
			{
				counter = document.getElementById("myRange2").value;
				id.innerHTML="STOPPED";
			}
		},1000);
	}
}

function JSONmessage(var_ObjectID,var_message)
{
	if (var_message== false) //used when binary valeues are requested sent
	{
    var JSONmessage=
	{		
	ObjectID:var_ObjectID,
	message:"No message",
	};
	var myJSONString = JSON.stringify(JSONmessage);
	doSend(myJSONString);		
	}
	else
	{
    var JSONmessage=
	{		
	ObjectID:var_ObjectID,
	message:var_message,
	};
	var myJSONString = JSON.stringify(JSONmessage);
	doSend(myJSONString);
	}
	
}

function SetPWM() {
	var Pumpflow = document.getElementById("myRange").value;
	JSONmessage("sendPumpFlow",Pumpflow);
	/*
	var JSONmessage=
	{		
	ObjectID:"sendPumpFlow",
	message:Pumpflow,
	};
	var myJSONString = JSON.stringify(JSONmessage);
	doSend(myJSONString);*/
}
	
function Settime() {
	var Pumptime = document.getElementById("myRange2").value;
	JSONmessage("sendPumpTime",Pumptime,);
	/*
	var JSONmessage=
	{		
	ObjectID:"sendPumpTime",
	message:Pumptime,
	};
	var myJSONString = JSON.stringify(JSONmessage);
	doSend(myJSONString);*/
}

// This is called when the page finishes loading
function init() {
	
    // Assign page elements to variables
    button = document.getElementById("myCheck");
    canvas = document.getElementById("pumpStatus");
    
    // Draw circle in canvas
    context = canvas.getContext("2d");
    context.arc(25, 25, 15, 0, Math.PI * 2, false);
    context.lineWidth = 3;
    context.strokeStyle = "black";
    context.stroke();
    context.fillStyle = "black";
    context.fill();
    // Connect to WebSocket server
    wsConnect(url_socket);
}
	
// Call this to connect to the WebSocket server
function wsConnect(url) {    
    // Connect to WebSocket server
    conn = new WebSocket(url);
    // Assign callbacks
    conn.onopen = function(evt){onOpen(evt)};
    conn.onclose = function(evt){onClose(evt)};
    conn.onmessage = function(evt){onMessage(evt)};
    conn.onerror = function(evt){onError(evt)};
}

// Called when a WebSocket connection is established with the server
function onOpen(evt) {
 
    // Log connection state
    console.log("Connected");
	//doSend("getPumpState");
	JSONmessage("getPumpState",false);
	SetPWM();
	Settime();
    //doSend("getPumpState");
	//doSend("getPumpTime");
	//doSend("getPumpFlow");
}

// Called when the WebSocket connection is closed
function onClose(evt) {
    // Log disconnection state
    console.log("Disconnected");
    // Try to reconnect after a few seconds

    setTimeout(function() {wsConnect(url_socket)},2000);   
}
// Called when a message is received from the server
function onMessage(evt) {
    // Print out our received message
    console.log("Received: " + evt.data);   
    // Update circle graphic with LED state
    switch(evt.data) {
        case "0":
            console.log("Pump is off");
            context.fillStyle = "black";
            context.fill();
            break;
        case "1":
            console.log("Pump is on");
            context.fillStyle = "red";
            context.fill();
            break;
        default:
            break;
    }
}
 
// Called when a WebSocket error occurs
function onError(evt) {
    console.log("ERROR: " + evt.data);
}
 
// Sends a message to the server (and prints it to the console)
function doSend(message) {
    console.log("Sending: " + message);
    //conn.onopen = () => conn.send(message);
	conn.send(message);
}
 
function onPress() {
    //doSend("togglePump");
	JSONmessage("togglePump",false,);
    JSONmessage("getPumpState",false);
}	
// Call the init function as soon as the page loads
window.addEventListener("load", init, false);

