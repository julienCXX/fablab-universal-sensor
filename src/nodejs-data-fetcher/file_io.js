// File I/O handling

var fs = require('fs');

var dualOutput = true;
var automaticFileName = false;
var curDate = null;
var curFilePath = '';
var curFileName = 'out.dat';
var curFileIndex = 0;


// see: http://stackoverflow.com/questions/8043026/javascript-format-number-to-have-2-digit
var leftPad = function(number, targetLength) {
    var output = number + '';
    while (output.length < targetLength) {
        output = '0' + output;
    }
    return output;
}

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

var getTodayDatePrefix = function() {
    return curDate.getFullYear() + '-' + leftPad(curDate.getMonth() + 1, 2)
        + '-' + leftPad(curDate.getDate(), 2);
}

var isAvailableFile = function(fNameAndPath) {
    if (fs.existsSync(fNameAndPath)) {
        return false;
    }
    return true;
}

var setIndexToNextAvailable = function() {
    var isAvailable = false;
    var datePrefix = getTodayDatePrefix();
    var fileIndex = curFileIndex;
    while (!isAvailable) {
        if (isAvailableFile(curFilePath
                    + getFileNameWithSuffix(datePrefix, fileIndex))) {
            isAvailable = true;
        } else {
            fileIndex++;
        }
    }
    curFileIndex = fileIndex;
}

var getFileNameWithSuffix = function(fileNamePrefix, suffix) {
    return fileNamePrefix + '_' + leftPad(suffix, 3) + '.dat';
}

var sameDay = function(date1, date2) {
    return date1.getDate() === date2.getDate();
}

var updateCurFileNameIfRequired = function() {
    if (!automaticFileName) {
        return;
    }
    var nowDate = new Date();
    if (curDate === null || !sameDay(nowDate, curDate)) {
        curDate = nowDate;
        curFileIndex = 0;
        setIndexToNextAvailable();
        curFileName = getFileNameWithSuffix(getTodayDatePrefix(), curFileIndex);
    }
}

setManualFileNameAndPath = function(fileNameAndPath) {
    automaticFileName = false;
    curFilePath = '';
    curFileName = fileNameAndPath;
}

setAutomaticFilePath = function(filePath) {
    automaticFileName = true;
    curFilePath = filePath;
    var length = curFilePath.length;
    if (length === 0 || curFilePath.charAt(length - 1) !== '/') {
        curFilePath += '/';
    }
    updateCurFileNameIfRequired();
}

changeCurFileNameToNextIndex = function() {
    if (!automaticFileName) {
        return;
    }
    setIndexToNextAvailable();
    curFileName = getFileNameWithSuffix(getTodayDatePrefix(), curFileIndex);
}

appendToCurrFile = function(string) {
    updateCurFileNameIfRequired();
    appendTo(curFilePath + curFileName, string);
}

