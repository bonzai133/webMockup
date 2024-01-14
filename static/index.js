

const temperatureRefreshTime = 10000; // Refresh temperture every 10 seconds
var temperature; // HTML element that incorporates the current temperature value
var unit; // HTML element for temperature unit
var tempContainer; // HTML element for temperature container
var mode; // HTML element for mode
var pump; // HTML element for pump
var formConfig; // HTML element for config form

window.addEventListener('load', onLoad);


function onLoad(event) {
    initTime();
    initHomeStatus();
    initConfigForm();
    initAdvancedConfigForm();
    initConfigTimeSlots();
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

async function initConfigForm() {
    // Get the form element from the DOM
    formConfig = document.getElementById('config-form');
    if (formConfig) {
        // Load values
        const response = await fetch('/api/config');
        const data = await response.json();
      
        formConfig.elements['mode-select'].value = data.mode;
        formConfig.elements.Confort.value = data.confort;
        formConfig.elements.Deltac.value = data.deltaConfort;
        formConfig.elements.Eco.value = data.eco;
        formConfig.elements.Deltae.value = data.deltaEco;
        formConfig.elements.HorsGel.value = data.horsGel;
        formConfig.elements.Deltah.value = data.deltaHorsGel;

        // Add an event listener for the form submission
        formConfig.addEventListener('submit', async (event) => {
            event.preventDefault(); // prevent the form from being submitted normally

            // Get the form values and store them in an object
            const mode = formConfig.elements['mode-select'].value;
            const confort = Number(formConfig.elements.Confort.value);
            const deltaConfort = Number(formConfig.elements.Deltac.value);
            const eco = Number(formConfig.elements.Eco.value);
            const deltaEco = Number(formConfig.elements.Deltae.value);
            const horsGel = Number(formConfig.elements.HorsGel.value);
            const deltaHorsGel = Number(formConfig.elements.Deltah.value);
            
            const data = {
                mode: mode,
                confort: confort,
                deltaConfort: deltaConfort,
                eco: eco,
                deltaEco: deltaEco,
                horsGel: horsGel,
                deltaHorsGel: deltaHorsGel
            };

            try {
                // Send a POST request to the "/config" endpoint with the form data as a JSON payload
                const response = await fetch('/api/config', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: JSON.stringify(data)
                });
        
                // If the response is not ok, throw an error
                if (!response.ok) {
                    throw new Error('Configuration non appliquée !');
                }
        
                // Display a success message to the user
                alert('Configuration appliquée avec succès !');
            } catch (error) {
                console.error(error);
                alert('Erreur indéterminée lors de la sauvegarde de la configuration !');
            }
        });
    }

}

async function initAdvancedConfigForm() {
    // Get the form element from the DOM
    formAdvancedConfig = document.getElementById('advanced-form');
    if (formAdvancedConfig) {
        // Load values
        const response = await fetch('/api/advancedConfig');
        const data = await response.json();
      
        for (let key in data) {
            if (data.hasOwnProperty(key)) {
              const input = formAdvancedConfig.elements[key];
              if (input) {
                input.value = data[key];
              }
            }
        }
          
        // Add an event listener for the form submission
        formAdvancedConfig.addEventListener('submit', async (event) => {
            event.preventDefault(); // prevent the form from being submitted normally

            // Get the form values and store them in an object
            const formData = new FormData(formAdvancedConfig);

            const data = {};
            for (let [key, value] of formData.entries()) {
              data[key] = value;
            }

            try {
                // Send a POST request to the "/advancedConfig" endpoint with the form data as a JSON payload
                const response = await fetch('/api/advancedConfig', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: JSON.stringify(data)
                });
        
                // If the response is not ok, throw an error
                if (!response.ok) {
                    throw new Error('Configuration non appliquée !');
                }
        
                // Display a success message to the user
                alert('Configuration appliquée avec succès !');
            } catch (error) {
                console.error(error);
                alert('Erreur indéterminé lors de la sauvegarde de la configuration !');
            }
        });
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

function initHomeStatus() {
    // Only in homepage
    if (document.getElementById('home-status')) {
        temperature = document.getElementById('temperature');
        unit = document.getElementById('unit');
        tempContainer = document.getElementById('tempContainer');
        mode = document.getElementById('mode');
        pump = document.getElementById('pump');

        setTemperature("-");
        getHomeStatus();
        setInterval(getHomeStatus, temperatureRefreshTime);
    }

}

function getHomeStatus() {
    asyncAwaitRequest('/api/homeStatus/', (rspDoc) => {
        setTemperature(rspDoc.temperature);

        switch(rspDoc.mode) {
            case 'prog': mode.innerText = "Planifié"; break;
            case 'confort': mode.innerText = "Confort"; break;
            case 'eco': mode.innerText = "Economique"; break;
            case 'horsgel': mode.innerText = "Hors gel"; break;
            default: mode.innerText = ""; break;
        }

        pump.innerText = rspDoc.pump;
    });
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

function deleteTimeSlot(button) {
    button.parentNode.remove();
}

function addTimeSlot(button, start="00:00", end="00:00") {
    const div = document.createElement('div');
    div.className = 'time-slot';
    div.innerHTML = `<input type="time" name="timeStart" class="edit-input" value="${start}"> <input type="time" name="timeEnd" class="edit-input" value="${end}"> <button type="button" class="delete-button" onclick="deleteTimeSlot(this)">Supprimer</button>`;
    // Insert Before
    button.parentNode.insertBefore(div, button);
    // Insert After
    //button.parentNode.insertBefore(div, button.nextSibling);

    return div;
}

function submitTimeSlots() {
    const days = ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'];
    const tbody_tr = document.getElementById('timeSlotForm_tbody_tr');
    const timeSlots = [];

    const cells = tbody_tr.getElementsByTagName('td');
    for (let i = 0; i < cells.length; i++) {
        const day = days[i];
        const times = [];

        cells[i].querySelectorAll('input[name="timeStart"]').forEach((timeStartInput, index) => {
            const timeEndInput = cells[i].querySelectorAll('input[name="timeEnd"]')[index];

            if (timeStartInput && timeEndInput) {
                const timeStart = timeStartInput.value;
                const timeEnd = timeEndInput.value;
                times.push({ start: timeStart, end: timeEnd });
            }
        });
        timeSlots.push({ day: day, times: times} );
    }
    
     // Send timeSlots to API
    fetch('/api/timeSlots', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ timeSlots })
    })
    .then(response => {
        if (response.ok) {
            alert('Plages horaires envoyées avec succès à l\'API.');
        } else {
            alert('Échec de l\'envoi des plages horaires à l\'API.');
        }
    })
    .catch(error => {
        console.error('Erreur lors de l\'envoi des données à l\'API:', error);
    });
}


async function initConfigTimeSlots() {
    const timeSlotForm = document.getElementById('timeSlotForm');
    if (timeSlotForm) {
        // Load values
        const response = await fetch('/api/timeSlots');
        const data = await response.json();
      
        data.timeSlots.forEach(
            ts => {
                const button = document.getElementById('button_' + ts.day);

                // Get times start, end
                ts.times.forEach(
                    period => {
                        div = addTimeSlot(button, period.start, period.end);
                    }
                );
            }
        );
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