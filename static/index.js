
window.addEventListener('load', onLoad);

function onLoad(event) {
    initTime();
}

function initTime() {
    time = document.getElementById('time');
    updateTime();
    // a timing event manager is initialized
    // which must be triggered every second to refresh
    // the display of the current time
    setInterval(updateTime, 1000);
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
