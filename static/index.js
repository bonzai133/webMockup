

const temperatureRefreshTime = 10000; // Refresh temperture every 10 seconds
var temperature; // HTML element that incorporates the current temperature value
var unit; // HTML element for temperature unit
var tempContainer; // HTML element for temperature container

window.addEventListener('load', onLoad);


function onLoad(event) {
    initTime();
    initTemperature();
}

function initTime() {
    // Time is not available on all pages
    time = document.getElementById('time');
    if(time !== null) {
        updateTime();
        // a timing event manager is initialized
        // which must be triggered every second to refresh
        // the display of the current time
        setInterval(updateTime, 1000);
    }
}

function updateTime() {
    let now    = new Date();
    let h      = now.getHours();
    let m      = now.getMinutes();
    let s      = now.getSeconds();
    time.innerHTML = `${normalize(h)}:${normalize(m)}:${normalize(s)}`;
}

function normalize(digit) {
    return (digit < 10 ? '0' : '') + digit;
}

function initTemperature() {
    // temperature is not available on all pages
    temperature = document.getElementById('temperature');
    if (temperature !== null) {
        unit = document.getElementById('unit');
        tempContainer = document.getElementById('tempContainer');

        setTemperature("-");
        getTemperature();
        setInterval(getTemperature, temperatureRefreshTime);
    }
}

function getTemperature() {
    asyncAwaitRequest('/temperature/', (tempDoc) => {
        temp = tempDoc.temperature;
        setTemperature(temp); });
}

function setTemperature(temp) {
    if (temp == 'Error' || temp == '-') {
        temperature.innerText = temp;
        unit.style.display = 'none';

        if(!tempContainer.classList.contains('error')) {
            tempContainer.classList.add('error');
        }
    } else {
        let t   = Number.parseFloat(temp).toFixed(1);

        temperature.innerText = t;
        unit.style.display = 'inline';

        if(tempContainer.classList.contains('error')) {
            tempContainer.classList.remove('error');
        }  
        /*
        if (t < low) {
            screen.className = 'cold';
        } else if (t > upp) {
            screen.className = 'hot';
        } else {
            screen.className = '';
        }
        */
    }
}

// Using Async/Await Promises
// @see: https://medium.com/@mattburgess/how-to-get-data-with-javascript-in-2018-f30ba04ad0da

function asyncAwaitRequest(path, callback) {
    (async () => {
        let data = "Error";
        try {
            let response = await fetch(path);
            data = await response.json();
        } catch(err) {
            data = "Error";
        }

        // callback is optional!
        typeof callback === 'function' && callback(data);
    })();
}