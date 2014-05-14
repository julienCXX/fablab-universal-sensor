// File I/O handling

var fs = require('fs');

var dualOutput = true;

var appendTo = function(file, string) {
    if (file === '' || file === null || file === undefined ) {
        console.log(string);
    } else {
        fs.appendFile(file, string + '\n', function (err) {
            if (err) throw err;
        });
        if (dualOutput) {
            console.log(string);
        }
    }
}

appendToCurrFile = function(string) {
    appendTo('out.dat', string);
}

