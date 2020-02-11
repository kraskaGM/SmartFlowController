//Variables
var url = "ws://192.168.1.1:1337/";
var button;
var canvas;
var context;
var flow = 0;
var pump = 0;

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
			id.innerHTML="Stopped";
				counter = document.getElementById("myRange2").value;
			}
		},1000);
	}
}

function sendlow(postflow) {
	var someUrl = "/postflow";
	$.ajax({
		url: someUrl,
		dataType: "text",
		success: function(response) {flow = parseInt(response);},
		timeout: 2000
		}
	)
}
function Setflow(postflow) {
	var FlowRate =document.getElementById("myRange").value;
	var someUrl = "/postflow/update?value=" + FlowRate;
	$.ajax({url: someUrl,dataType: "text",success: function(response) {flow = parseInt(response);}})
}
	
var pump = 0;
function sendtime(postime) {
	var someUrl  = "/postime";
	$.ajax({
		url: someUrl,
		dataType: "text",
		success: function(response) {pump = parseInt(response);},
		timeout: 2000
		}
	)
}
function Settime(postime) {
	var Pumptime = document.getElementById("myRange2").value;	
	var someUrl = "/postime/update?value=" + Pumptime;
	$.ajax({url: someUrl,dataType: "text",success: function(response) {pump = parseInt(response);}})
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
    wsConnect(url);
}
	
// Call this to connect to the WebSocket server
function wsConnect(url) {    
    // Connect to WebSocket server
    websocket = new WebSocket(url);
    // Assign callbacks
    websocket.onopen = function(evt) { onOpen(evt) };
    websocket.onclose = function(evt) { onClose(evt) };
    websocket.onmessage = function(evt) { onMessage(evt) };
    websocket.onerror = function(evt) { onError(evt) };
}

// Called when a WebSocket connection is established with the server
function onOpen(evt) {
 
    // Log connection state
    console.log("Connected");
    
    // Get the current state of the Pump
    doSend("getPumpState");
}

// Called when the WebSocket connection is closed
function onClose(evt) {
    // Log disconnection state
    console.log("Disconnected");
    // Try to reconnect after a few seconds
    setTimeout(function() { wsConnect(url) }, 2000);
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
    websocket.send(message);
}
 
function onPress() {
    doSend("togglePump");
    doSend("getPumpState");
}
 
// Call the init function as soon as the page loads
window.addEventListener("load", init, false);
