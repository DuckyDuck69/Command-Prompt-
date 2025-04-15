

//listen to the connect event on the server js file 
document.addEventListener("DOMContentLoaded", ()=>{
    const socket = io();

    socket.on("connection", ()=>{
        console.log("Server is running");
        console.log(`${socket.id} is connected!`)
    })  

    //listen to an event, when it happens run the function 
    socket.on()
})
