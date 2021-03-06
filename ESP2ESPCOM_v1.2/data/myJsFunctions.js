//Variables
var url_socket = "ws://192.168.5.1/ws";
var button;
var canvas;
var context;
var flow = 0;
var pump = 0;
var conn;

//Reference points
var A=[1096,11.96];
var B=[1595,23.902];
var C=[2094,29.858];
var D=[2594,41.192];
var E=[3094,49.372];
var F=[3594,61.268];
var G=[4095,72.93];

//MAC Addresses
var MACAdress2=[0x4C, 0x11, 0xAE, 0xEA, 0xEE, 0xB4];
var MACAdress3=[0xff,0xff,0xff,0xff,0xff,0xff];
//Converting ml/min to analog value
//Calibration coefficiant
var calibration = 100;

function mlmintoAnalog(input)
{
	var calculate=0;
	if (input>11.96 && input<23.902)
	{
		calculate= ((input-A[1])*(B[0]-A[0])/(B[1]-A[1]))+A[0];
	}
	else if (input>23.902 && input<29.858)
	{
		calculate= ((input-B[1])*(C[0]-B[0])/(C[1]-B[1]))+B[0];
	}
	else if (input>29.858 && input<41.192)
	{
		calculate= ((input-C[1])*(D[0]-C[0])/(D[1]-C[1]))+C[0];
	}
	else if (input>41.192 && input<49.372)
	{
		calculate= ((input-D[1])*(E[0]-D[0])/(E[1]-D[1]))+D[0]-calibration;
	}	
	else if (input>49.372 && input<61.268)
	{
		calculate= ((input-E[1])*(F[0]-E[0])/(F[1]-E[1]))+E[0]-calibration;
	}	
	else if (input>61.268 && input<72.93)
	{
		calculate= ((input-F[1])*(G[0]-F[0])/(G[1]-F[1]))+F[0]-calibration;
	}		
	
	return Math.round(calculate);
}
// Called whenever the HTML button is pressed
function changeOutput(input1, input2) {
	onPress(input1);
	counter(input2);
}
function ConnectionStatus(input)
{
	switch(input) {
		case 1:
		{
			MACAdress2[0]= document.getElementById("p2MAC1").value;
			MACAdress2[1]= document.getElementById("p2MAC2").value;
			MACAdress2[2]= document.getElementById("p2MAC3").value;
			MACAdress2[3]= document.getElementById("p2MAC4").value;
			MACAdress2[4]= document.getElementById("p2MAC5").value;
			MACAdress2[5]= document.getElementById("p2MAC6").value;			
			
			console.log(MACAdress2);
			//var field = document.getElementById("ConnectionStatus1").value;				
			id = document.getElementById("ConnectionStatus1");
			id.innerHTML="Connection established";
		break;
		}
		case 2: 
		{
			MACAdress3[0]= document.getElementById("p3MAC1").value;
			MACAdress3[1]= document.getElementById("p3MAC2").value;
			MACAdress3[2]= document.getElementById("p3MAC3").value;
			MACAdress3[3]= document.getElementById("p3MAC4").value;
			MACAdress3[4]= document.getElementById("p3MAC5").value;	
			MACAdress3[5]= document.getElementById("p3MAC6").value;	
			console.log(MACAdress3);		
			//var field = document.getElementById("ConnectionStatus1").value;				
			id = document.getElementById("ConnectionStatus2");
			id.innerHTML="Connection established";	
		break;		
		}
	}
}
function counter(input)
{
	switch(input) {
	case 1:
		var x = document.getElementById("myCheck1").checked;
		var y = document.getElementById("Pump1Slider_2").value;
		if (x==true) 
		{		  
			var counter = y;
			console.log("Pump is on");
			context.fillStyle = "red";
			context.fill();
			setInterval(function()
			{
				counter--;
				x = document.getElementById("myCheck1").checked;				
				if (counter > 0 & x==true)
				{
					id = document.getElementById("count1");
					id.innerHTML=counter;
				}
				else if (counter == 0 & x==true)
				{
					id.innerHTML="COMPLETE";
					console.log("Pump finished job");
					context.fillStyle = "green";
					context.fill();
				$(document).ready(function()
				{
					$("#myCheck1").prop("checked", false);
				});

				}
				else if (x==false) 
				{
					counter = document.getElementById("Pump1Slider_2").value;
					id.innerHTML="STOPPED";
					console.log("Pump is off");
					context.fillStyle = "black";
					context.fill();
				}
			},1000);
		}
		break;
	case 2:
		var x = document.getElementById("myChec2").checked;
		var y = document.getElementById("Pump2Slider_2").value;
		if (x==true) 
		{		  
			var counter = y;
			console.log("Pump is on");
			context.fillStyle = "red";
			context.fill();
			setInterval(function()
			{
				counter--;
				x = document.getElementById("myCheck2").checked;				
				if (counter > 0 & x==true)
				{
					id = document.getElementById("count2");
					id.innerHTML=counter;
				}
				else if (counter == 0 & x==true)
				{
					id.innerHTML="COMPLETE";
					console.log("Pump finished job");
					context.fillStyle = "green";
					context.fill();
				$(document).ready(function()
				{
					$("#myCheck2").prop("checked", false);
				});

				}
				else if (x==false) 
				{
					counter = document.getElementById("Pump1Slider_2").value;
					id.innerHTML="STOPPED";
					console.log("Pump is off");
					context.fillStyle = "black";
					context.fill();
				}
			},1000);
		}
		break;
	case 3:
		var x = document.getElementById("myCheck3").checked;
		var y = document.getElementById("Pump3Slider_2").value;
		if (x==true) 
		{		  
			var counter = y;
			console.log("Pump is on");
			context.fillStyle = "red";
			context.fill();
			setInterval(function()
			{
				counter--;
				x = document.getElementById("myCheck3").checked;				
				if (counter > 0 & x==true)
				{
					id = document.getElementById("count3");
					id.innerHTML=counter;
				}
				else if (counter == 0 & x==true)
				{
					id.innerHTML="COMPLETE";
					console.log("Pump finished job");
					context.fillStyle = "green";
					context.fill();
				$(document).ready(function()
				{
					$("#myCheck3").prop("checked", false);
				});

				}
				else if (x==false) 
				{
					counter = document.getElementById("Pump3Slider_2").value;
					id.innerHTML="STOPPED";
					console.log("Pump is off");
					context.fillStyle = "black";
					context.fill();
				}
			},1000);
	}
	break;
	default:
		break;
	}
	
}

function ShowHide(input) {
	switch(input) {
	  case 1:
		  ConnectionStatus(1);
		  JSONmessage("sendMACAdress2",MACAdress2);
		  //JSONmessage("sendMACAdress2",true);
		  var x = document.getElementById("myDIV");
		  if (x.style.display === "block") {
		    x.style.display ="none" ;	
		  } else {
			x.style.display = "block";
	  }
		break;
	  case 2:
		  ConnectionStatus(2);
		  JSONmessage("sendMACAdress3",MACAdress3);
		  //JSONmessage("sendMACAdress2",true);		  
		  var x = document.getElementById("myDIV2");
		  if (x.style.display === "block") {
			x.style.display ="none" ;
		  } else {
			x.style.display = "block";	
		  }
		break;
	  default:
			//JSONmessage("sendMACAdress2",false);
			//JSONmessage("sendMACAdress2",false);
		break;
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

function SetPWM(input) {
	switch(input) {
	  case 1:
		var Pumpflow = document.getElementById("Pump1Slider_1").value;
		var Conversion =mlmintoAnalog(Pumpflow);
		JSONmessage("sendPumpFlow1",Conversion);
		break;
	  case 2:
		var Pumpflow = document.getElementById("Pump2Slider_1").value;
		var Conversion =mlmintoAnalog(Pumpflow);
		JSONmessage("sendPumpFlow2",Conversion);
		break;
	  case 3:
		var Pumpflow = document.getElementById("Pump3Slider_1").value;
		var Conversion =mlmintoAnalog(Pumpflow);
		JSONmessage("sendPumpFlow3",Conversion);
		break;
	  default:

		JSONmessage("None",0);
		break;
	}

	/*
	var JSONmessage=
	{		
	ObjectID:"sendPumpFlow",
	message:Pumpflow,
	};
	var myJSONString = JSON.stringify(JSONmessage);
	doSend(myJSONString);*/
}
	
function Settime(input) {
	switch(input) {
	  case 1:
		var Pumptime = document.getElementById("Pump1Slider_2").value;
		JSONmessage("sendPumpTime1",Pumptime);
		break;
	  case 2:
		var Pumptime = document.getElementById("Pump2Slider_2").value;
		JSONmessage("sendPumpTime2",Pumptime);
		break;
	  case 3:
		var Pumptime = document.getElementById("Pump3Slider_2").value;
		JSONmessage("sendPumpTime3",Pumptime);
		break;
	  default:
		JSONmessage("None",0);
		break;
	}		
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
    button = document.getElementById("myCheck1");
    canvas = document.getElementById("pumpStatus1");
    
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
	JSONmessage("getPumpState1",false);
	SetPWM(1);
	Settime(1);
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
	/*
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
    }*/
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
 
function onPress(input) {
    //doSend("togglePump");

	switch(input) {
	  case 1:
		JSONmessage("togglePump1",false,);
		JSONmessage("getPumpState1",false);
		break;
	  case 2:
		JSONmessage("togglePump2",false,);
		JSONmessage("getPumpState2",false);
		break;
	  case 3:
		JSONmessage("togglePump3",false,);
		JSONmessage("getPumpState3",false);
		break;
	  default:
		JSONmessage("None",0);
		break;
	}
}

// Call the init function as soon as the page loads
window.addEventListener("load", init, false);
window.onload=function()
{
//Refresh submit disable
function handleForm(event) 
{ 
	event.preventDefault();
} 	
//Pump 1  Disable refresh
var form = document.getElementById("myForm1");
form.addEventListener('submit', handleForm);


// Disable refresh 2
var form2 = document.getElementById("myForm2");
form2.addEventListener('submit', handleForm);

// Pump 1 Listeners 1
document.getElementById("Pump1Slider_1").addEventListener("click", function(){SetPWM(1)});
document.getElementById("Pump1Slider_1").addEventListener("touchend", function(){SetPWM(1)});
var slider = document.getElementById("Pump1Slider_1");
var output = document.getElementById("Slider1Value");
output.innerHTML = slider.value;
slider.oninput = function() 
{
  output.innerHTML = this.value;
}
// Pump 1 Listeners 2
document.getElementById("Pump1Slider_2").addEventListener("click", function(){Settime(1)});
document.getElementById("Pump1Slider_2").addEventListener("touchend", function(){Settime(1)});
var slider2 = document.getElementById("Pump1Slider_2");
var output2 = document.getElementById("Slider2Value");
output2.innerHTML = slider2.value;
slider2.oninput = function() 
{
  output2.innerHTML = this.value;
}
// Pump 1 Listeners 3
document.getElementById("myCheck1").addEventListener("click", function(){changeOutput(1,1)});
document.getElementById("myCheck1").addEventListener("touchend", function(){changeOutput(1,1)});



// Pump 2 Listeners 1
document.getElementById("Pump2Slider_1").addEventListener("click", function(){SetPWM(2)});
document.getElementById("Pump2Slider_1").addEventListener("touchend", function(){SetPWM(2)});
var slider3 = document.getElementById("Pump2Slider_1");
var output3 = document.getElementById("Slider3Value");
output3.innerHTML = slider3.value;
slider3.oninput = function() {
  output3.innerHTML = this.value;
}

//Pump 2 Listeners 2
document.getElementById("Pump2Slider_2").addEventListener("click", function(){Settime(1)});
document.getElementById("Pump2Slider_2").addEventListener("touchend", function(){Settime(1)});
var slider4 = document.getElementById("Pump2Slider_2");
var output4 = document.getElementById("Slider4Value");
output4.innerHTML = slider4.value;
slider4.oninput = function() {
  output4.innerHTML = this.value;
}
// Pump 2 Listener 3
document.getElementById("myCheck2").addEventListener("click", function(){changeOutput(2,2)});
document.getElementById("myCheck2").addEventListener("touchend", function(){changeOutput(2,2)});
	

// Pump 3 listeners 1
document.getElementById("Pump3Slider_1").addEventListener("click", function(){SetPWM(3)});
document.getElementById("Pump3Slider_1").addEventListener("touchend", function(){SetPWM(3)});
var slider5 = document.getElementById("Pump3Slider_1");
var output5 = document.getElementById("Slider5Value");
output5.innerHTML = slider5.value;
slider5.oninput = function() {
  output5.innerHTML = this.value;
}
// Pump 3 listeners 2
document.getElementById("Pump3Slider_2").addEventListener("click", function(){Settime(1)});
document.getElementById("Pump2Slider_2").addEventListener("touchend", function(){Settime(1)});
var slider6 = document.getElementById("Pump3Slider_2");
var output6 = document.getElementById("Slider6Value");
output6.innerHTML = slider6.value;
slider6.oninput = function() {
output6.innerHTML = this.value;
}

//Listeners 6
document.getElementById("myCheck3").addEventListener("click", function(){changeOutput(3,3)});
document.getElementById("myCheck3").addEventListener("touchend", function(){changeOutput(3,3)});
}
