// Interrupt signal handling

// see http://zguide.zeromq.org/js:interrupt

var zmq = require('zmq'), socket = zmq.createSocket('rep');

var sigIntCallback = function() {
    console.log('SIGINT received');
    process.exit(0);
};

socket.on('message', function(buf) {
    // echo request back
    socket.send(buf);
});

process.on('SIGINT', function() {
    socket.close();
    sigIntCallback();
});

setSigIntCallback = function(callback) {
    sigIntCallback = callback;
}

