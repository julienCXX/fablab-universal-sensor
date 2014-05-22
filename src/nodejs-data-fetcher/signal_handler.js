// Interrupt signal handling

// see http://zguide.zeromq.org/js:interrupt

var sigIntCallback = function() {
    console.log('SIGINT received');
    process.exit(0);
};

process.on('SIGINT', function() {
    sigIntCallback();
});

setSigIntCallback = function(callback) {
    sigIntCallback = callback;
}

