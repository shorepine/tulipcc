import { ESPLoader, Transport } from "https://unpkg.com/esptool-js@0.5.4/bundle.js";
//import { ESPLoader, Transport } from "./esptool-js/bundle.js";

const baudRates = [921600, 115200, 230400, 460800];

const maxLogLength = 100;
const log = document.getElementById("log");
const butConnect = document.getElementById("butConnect");
const baudRate = document.getElementById("baudRate");
const butClear = document.getElementById("butClear");
const butErase = document.getElementById("butErase");
const butProgram = document.getElementById("butProgram");
const autoscroll = document.getElementById("autoscroll");
const lightSS = document.getElementById("light");
const darkSS = document.getElementById("dark");
const progress = document.querySelectorAll(".upload .progress-bar");
const offsets = document.querySelectorAll(".upload .offset");
const appDiv = document.getElementById("app");
const noReset = document.getElementById("noReset");

const upgradeActions = [
    {
        button: butProgram,
        offsetInput: offsets[0],
        url: butProgram?.dataset?.url,
        eraseAll: false,
        eraseOTA: true,
    },
    {
        button: butErase,
        offsetInput: offsets[1],
        url: butErase?.dataset?.url,
        eraseAll: true,
        eraseOTA: false,
    }
];

let device = null;
let transport = null;
let esploader = null;
let chip = null;
const serialLib = !navigator.serial && navigator.usb ? serial : navigator.serial;

document.addEventListener("DOMContentLoaded", () => {
    if (butConnect) {
        butConnect.addEventListener("click", () => {
            clickConnect().catch(async (e) => {
                errorMsg(e.message || e);
                toggleUIConnected(false);
            });
        });
    }
    if (butClear) {
        butClear.addEventListener("click", clickClear);
    }
    if (butErase) {
        butErase.addEventListener("click", () => flashFromUrl(1));
    }
    if (butProgram) {
        butProgram.addEventListener("click", () => flashFromUrl(0));
    }
    if (autoscroll) {
        autoscroll.addEventListener("click", clickAutoscroll);
    }
    if (baudRate) {
        baudRate.addEventListener("change", changeBaudRate);
    }
    if (noReset) {
        noReset.addEventListener("change", clickNoReset);
    }

    window.addEventListener("error", function (event) {
        console.log("Got an uncaught error: ", event.error);
    });
    const notSupported = document.getElementById("notSupported");
    const supportsSerial =
        serialLib && typeof serialLib.requestPort === "function";
    if (supportsSerial) {
        notSupported.classList.add("hidden");
    } else {
        notSupported.classList.remove("hidden");
        document.querySelector(".header")?.classList.add("hidden");
        document.querySelector(".subheader")?.classList.add("hidden");
        document.querySelector(".footer")?.classList.add("hidden");
        document.getElementById("app")?.classList.add("hidden");
        const main = document.querySelector("main.main");
        if (main) {
            Array.from(main.children).forEach((child) => {
                if (child.id !== "notSupported") {
                    child.classList.add("hidden");
                }
            });
        }
    }

    initBaudRate();
    loadAllSettings();
    updateTheme();
    writeLogLine("ESP Web Flasher loaded.");
});

function initBaudRate() {
    if (!baudRate) {
        return;
    }
    for (let rate of baudRates) {
        var option = document.createElement("option");
        option.text = rate + " Baud";
        option.value = rate;
        baudRate.add(option);
    }
    baudRate.value = "115200";
}

function pruneLog() {
    // Remove old log content
    if (log.textContent.split("\n").length > maxLogLength + 1) {
        let logLines = log.innerHTML.replace(/(\n)/gm, "").split("<br>");
        log.innerHTML = logLines.splice(-maxLogLength).join("<br>\n");
    }

    if (autoscroll && autoscroll.checked) {
        log.scrollTop = log.scrollHeight;
    }
}

function writeLog(text) {
    log.innerHTML += text;
    pruneLog();
}

function writeLogLine(text) {
    writeLog(text + "<br>");
}

const espLoaderTerminal = {
    clean() {
        log.innerHTML = "";
    },
    writeLine(data) {
        writeLogLine(data);
    },
    write(data) {
        writeLog(data);
    },
};

function errorMsg(text) {
    writeLogLine('<span class="error-message">Error:</span> ' + text);
    console.error(text);
}

/**
 * @name updateTheme
 * Sets the theme to  Adafruit (dark) mode. Can be refactored later for more themes
 */
function updateTheme() {
    // Disable all themes
    document
        .querySelectorAll("link[rel=stylesheet].alternate")
        .forEach((styleSheet) => {
        enableStyleSheet(styleSheet, false);
        });

    enableStyleSheet(lightSS, true);
}

function enableStyleSheet(node, enabled) {
    node.disabled = !enabled;
}

/**
 * @name clickConnect
 * Click handler for the connect/disconnect button.
 */
async function clickConnect() {
    // Disconnect if connected
    if (transport !== null) {
        await transport.disconnect();
        await transport.waitForUnlock(1500);
        toggleUIConnected(false);
        transport = null;
        if (device !== null) {
            await device.close();
            device = null;
        }
        chip = null;
        return;
    }

    // Set up device and transport
    if (device === null) {
        device = await serialLib.requestPort({});
    }

    if (transport === null) {
        transport = new Transport(device, true);
    }

    try {
        const romBaudrate = baudRate ? parseInt(baudRate.value) : 115200;
        const loaderOptions = {
            transport: transport,
            baudrate: romBaudrate,
            terminal: espLoaderTerminal,
            debugLogging: false,
        };

        esploader = new ESPLoader(loaderOptions);

        let resetMode = "default_reset";
        if (noReset && noReset.checked) {
            resetMode = "no_reset";
            try {
                // Initiate passthrough serial setup
                await transport.connect(romBaudrate);
                await transport.disconnect();
                await sleep(350);
            } catch (e) {
            }
        }

        chip = await esploader.main(resetMode);

        // Temporarily broken
        // await esploader.flashId();
        toggleUIConnected(true);
        toggleUIToolbar(true);

    } catch (e) {
        console.error(e);
        errorMsg(e.message);
    }
    console.log("Settings done for :" + chip);
}

/**
 * @name changeBaudRate
 * Change handler for the Baud Rate selector.
 */
async function changeBaudRate() {
    if (baudRate && baudRates.includes(parseInt(baudRate.value))) {
        saveSetting("baudrate", baudRate.value);
    }
}

/**
 * @name clickAutoscroll
 * Change handler for the Autoscroll checkbox.
 */
async function clickAutoscroll() {
    saveSetting("autoscroll", autoscroll.checked);
}

/**
 * @name clickDarkMode
 * Change handler for the Dark Mode checkbox.
 */
/**
 * @name clickNoReset
 * Change handler for ESP32 co-processor boards
 */
async function clickNoReset() {
    saveSetting("noReset", noReset.checked);
}

async function flashFromUrl(actionIndex) {
    const action = upgradeActions[actionIndex];
    if (!action || !action.url || !action.offsetInput) {
        errorMsg("Upgrade action is not configured.");
        return;
    }
    if (!esploader) {
        errorMsg("Please connect to your device first.");
        return;
    }

    if (baudRate) {
        baudRate.disabled = true;
    }
    if (butErase) {
        butErase.disabled = true;
        butErase.classList.add("hidden");
    }
    if (butProgram) {
        butProgram.disabled = true;
        butProgram.classList.add("hidden");
    }
    const progressBar = progress[actionIndex];
    const progressFill = progressBar.querySelector("div");
    let progressStarted = false;
    progressBar.classList.remove("hidden");
    progressBar.classList.add("indeterminate");
    progressFill.style.width = "100%";

    try {
        if (action.eraseAll) {
            writeLogLine("Erasing flash memory. Please wait...");
            let stamp = Date.now();
            await esploader.eraseFlash();
            writeLogLine(
                "Finished. Took " + (Date.now() - stamp) + "ms to erase."
            );
        } else if (action.eraseOTA) {
            await eraseOTAPartitions();
        }

        writeLogLine("Fetching firmware...");
        const contents = await fetchBinaryString(action.url);
        const offset = parseInt(action.offsetInput.value, 16);
        const fileArray = [{ data: contents, address: offset }];

        const flashOptions = {
            fileArray: fileArray,
            flashSize: "keep",
            eraseAll: action.eraseAll,
            compress: true,
            reportProgress: (fileIndex, written, total) => {
                if (!progressStarted) {
                    progressStarted = true;
                    progressBar.classList.remove("indeterminate");
                }
                const percent = Math.floor((written / total) * 100);
                progressFill.style.width = percent + "%";
                if (percent >= 100) {
                    progressBar.classList.add("indeterminate");
                    progressFill.style.width = "100%";
                }
            },
            calculateMD5Hash: (image) =>
                CryptoJS.MD5(CryptoJS.enc.Latin1.parse(image)),
        };
        await esploader.writeFlash(flashOptions);
        progressBar.classList.add("indeterminate");
        progressFill.style.width = "100%";
        await sleep(400);
        writeLogLine("To run the new firmware, please reset your device.");
        await esploader.after();
    } catch (e) {
        console.error(e);
        errorMsg(e.message || e);
    } finally {
        progressBar.classList.add("hidden");
        progressBar.classList.remove("indeterminate");
        progressFill.style.width = "0";
        if (butErase) {
            butErase.disabled = false;
            butErase.classList.remove("hidden");
        }
        if (butProgram) {
            butProgram.disabled = false;
            butProgram.classList.remove("hidden");
        }
        if (baudRate) {
            baudRate.disabled = false;
        }
    }
}

async function fetchBinaryString(url) {
    const prefersProxy = shouldUseProxyFirst();
    const proxyPaths = prefersProxy ? ["/api/firmware"] : ["/proxy"];

    for (const path of proxyPaths) {
        const proxyUrl = buildProxyUrl(url, path);
        if (!proxyUrl) {
            continue;
        }
        try {
            return await fetchBinaryStringDirect(proxyUrl);
        } catch (e) {
        }
    }

    return await fetchBinaryStringDirect(url);
}

async function eraseOTAPartitions() {
    const otaNames = ["ota_0", "ota_1"];
    const partitionTable = await esploader.readFlash(0x8000, 0x1000);
    const entries = parsePartitionTable(partitionTable);
    const targets = entries.filter((entry) => otaNames.includes(entry.name));
    if (targets.length === 0) {
        throw new Error("No OTA partitions found in partition table.");
    }

    writeLogLine("Erasing OTA partitions...");
    for (const target of targets) {
        const alignedSize = alignToSector(target.size);
        writeLogLine(
            `Erasing ${target.name} at 0x${target.offset.toString(16)} (${alignedSize} bytes)`
        );
        await eraseRegion(target.offset, alignedSize);
    }
    writeLogLine("OTA partition erase complete.");
}

function parsePartitionTable(bytes) {
    const entries = [];
    const view = new DataView(bytes.buffer, bytes.byteOffset, bytes.byteLength);
    const entrySize = 32;
    for (let i = 0; i + entrySize <= bytes.length; i += entrySize) {
        const magic = view.getUint16(i, true);
        if (magic !== 0x50aa) {
            break;
        }
        const offset = view.getUint32(i + 4, true);
        const size = view.getUint32(i + 8, true);
        const nameBytes = bytes.subarray(i + 12, i + 28);
        const name = decodeAscii(nameBytes);
        entries.push({ name, offset, size });
    }
    return entries;
}

function decodeAscii(bytes) {
    let out = "";
    for (let i = 0; i < bytes.length; i++) {
        if (bytes[i] === 0) {
            break;
        }
        out += String.fromCharCode(bytes[i]);
    }
    return out;
}

function alignToSector(size) {
    const sector = 0x1000;
    return Math.ceil(size / sector) * sector;
}

async function eraseRegion(offset, size) {
    const payload = new Uint8Array(8);
    const view = new DataView(payload.buffer);
    view.setUint32(0, offset, true);
    view.setUint32(4, size, true);
    const timeout = esploader.timeoutPerMb(
        esploader.ERASE_REGION_TIMEOUT_PER_MB,
        size
    );
    await esploader.checkCommand(
        "erase region",
        esploader.ESP_ERASE_REGION,
        payload,
        undefined,
        timeout
    );
}

async function fetchBinaryStringDirect(url) {
    const response = await fetch(url, { cache: "no-store" });
    if (!response.ok) {
        throw new Error(`Failed to download firmware (${response.status})`);
    }
    const buffer = await response.arrayBuffer();
    const bytes = new Uint8Array(buffer);
    let binary = "";
    const chunkSize = 0x8000;
    for (let i = 0; i < bytes.length; i += chunkSize) {
        const chunk = bytes.subarray(i, i + chunkSize);
        binary += String.fromCharCode.apply(null, chunk);
    }
    return binary;
}

function buildProxyUrl(url, path = "/proxy") {
    if (!window.location || !window.location.origin) {
        return null;
    }
    const proxy = new URL(path, window.location.origin);
    proxy.searchParams.set("url", url);
    return proxy.toString();
}

function shouldUseProxyFirst() {
    if (!window.location || !window.location.hostname) {
        return false;
    }
    const host = window.location.hostname;
    return host !== "localhost" && host !== "127.0.0.1";
}

/**
 * @name clickClear
 * Click handler for the clear button.
 */
async function clickClear() {
    // reset();     Reset function wasnt declared.
    log.innerHTML = "";
}

function toggleUIToolbar(show) {
    for (let i = 0; i < progress.length; i++) {
        progress[i].classList.add("hidden");
        progress[i].querySelector("div").style.width = "0";
    }
    if (show) {
        appDiv.classList.add("connected");
    } else {
        appDiv.classList.remove("connected");
    }
    butErase.disabled = !show;
    butProgram.disabled = !show;
}

function toggleUIConnected(connected) {
    if (connected) {
        if (butConnect) {
            butConnect.classList.add("hidden");
        }
    } else {
        if (butConnect) {
            butConnect.classList.remove("hidden");
        }
        toggleUIToolbar(false);
    }
}

function loadAllSettings() {
    // Load all saved settings or defaults
    if (autoscroll) {
        autoscroll.checked = loadSetting("autoscroll", true);
    }
    if (baudRate) {
        baudRate.value = 115200;
        saveSetting("baudrate", 115200);
    }
    if (noReset) {
        noReset.checked = loadSetting("noReset", false);
    }
}

function loadSetting(setting, defaultValue) {
    let value = JSON.parse(window.localStorage.getItem(setting));
    if (value == null) {
        return defaultValue;
    }

    return value;
}

function saveSetting(setting, value) {
    window.localStorage.setItem(setting, JSON.stringify(value));
}

function sleep(ms) {
    return new Promise((resolve) => setTimeout(resolve, ms));
}
