#pragma once
#include <Arduino.h>

const char DASHBOARD_HTML[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0">
<title>Pulsar 180 GT — Dashboard</title>
<style>
*,*::before,*::after{margin:0;padding:0;box-sizing:border-box}
body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,'Helvetica Neue',Arial,sans-serif;background:#f0f2f5;color:#1e293b;min-height:100vh;display:flex;justify-content:center;padding:16px}
.container{max-width:480px;width:100%}
header{text-align:center;padding:20px 0 16px}
header h1{font-size:1.5rem;font-weight:800;letter-spacing:-0.5px;color:#0f172a}
header p{font-size:0.85rem;color:#64748b;margin-top:2px}
.card{background:#fff;border-radius:16px;padding:20px;margin-bottom:16px;box-shadow:0 1px 3px rgba(0,0,0,.06),0 1px 2px rgba(0,0,0,.04);transition:box-shadow .2s}
.card:hover{box-shadow:0 4px 12px rgba(0,0,0,.08)}
.status-card{text-align:center}
.status-indicator{display:inline-flex;align-items:center;gap:10px;background:#f8fafc;padding:10px 24px;border-radius:40px;margin-bottom:16px}
.status-dot{width:14px;height:14px;border-radius:50%;background:#94a3b8;transition:background .4s,box-shadow .4s}
.status-dot.connected{background:#10b981;box-shadow:0 0 8px rgba(16,185,129,.5)}
.status-dot.fair{background:#f59e0b;box-shadow:0 0 8px rgba(245,158,11,.5)}
.status-dot.poor{background:#ef4444;box-shadow:0 0 8px rgba(239,68,68,.5)}
#distanceLabel{font-size:1.1rem;font-weight:600}
.signal-bar-container{display:flex;align-items:center;gap:12px;margin-bottom:12px}
.signal-bar{flex:1;height:28px;background:#e2e8f0;border-radius:14px;overflow:hidden;position:relative}
.signal-fill{height:100%;border-radius:14px;background:linear-gradient(90deg,#ef4444,#f59e0b,#10b981);transition:width .6s ease;width:0%}
.signal-value{font-size:1rem;font-weight:700;color:#0f172a;white-space:nowrap;min-width:60px}
.distance-text{font-size:.9rem;color:#64748b}
.distance-text span{font-weight:600;color:#1e293b}
.distance-slider{padding:8px 0}
.slider-track{position:relative;height:6px;background:#e2e8f0;border-radius:3px;margin-bottom:8px}
.slider-fill{position:absolute;left:0;top:0;height:100%;border-radius:3px;background:linear-gradient(90deg,#10b981,#f59e0b,#ef4444);transition:width .6s ease}
.slider-thumb{position:absolute;top:50%;width:18px;height:18px;background:#fff;border:3px solid #2563eb;border-radius:50%;transform:translate(-50%,-50%);transition:left .6s ease;box-shadow:0 2px 6px rgba(37,99,235,.3)}
.slider-labels{display:flex;justify-content:space-between;font-size:.7rem;color:#94a3b8;margin-top:4px}
.stats-card{display:flex;gap:12px}
.stat{flex:1;text-align:center;padding:12px;background:#f8fafc;border-radius:12px}
.stat-label{display:block;font-size:.75rem;color:#94a3b8;margin-bottom:4px;text-transform:uppercase;letter-spacing:.5px}
.stat-value{display:block;font-size:1.3rem;font-weight:700;color:#0f172a}
.arm-btn{display:block;width:100%;padding:16px;border:none;border-radius:14px;background:#2563eb;color:#fff;font-size:1rem;font-weight:700;cursor:pointer;transition:background .2s,transform .1s;margin-bottom:16px}
.arm-btn:hover{background:#1d4ed8}
.arm-btn:active{transform:scale(.98)}
.arm-btn.armed{background:#ef4444}
.arm-btn.armed:hover{background:#dc2626}
.footer{text-align:center;padding:8px 0 24px;color:#94a3b8;font-size:.8rem;line-height:1.8}
.footer .version{font-size:.7rem;color:#cbd5e1;margin-top:2px}
@media(max-width:400px){.card{padding:16px}.status-indicator{padding:8px 18px}.stat-value{font-size:1.1rem}}
</style>
</head>
<body>
<div class="container">
<header>
<h1>PULSAR 180 GT</h1>
<p>Sistema de Proximidad</p>
</header>

<div class="card status-card">
<div class="status-indicator">
<div class="status-dot" id="statusDot"></div>
<span id="distanceLabel">Conectando...</span>
</div>
<div class="signal-bar-container">
<div class="signal-bar"><div class="signal-fill" id="signalFill"></div></div>
<span class="signal-value" id="rssiValue">--</span>
</div>
<p class="distance-text">Distancia estimada: <span id="distanceText">--</span></p>
</div>

<div class="card">
<h3 style="font-size:.85rem;color:#64748b;margin-bottom:12px;text-transform:uppercase;letter-spacing:.5px">Proximidad</h3>
<div class="distance-slider">
<div class="slider-track">
<div class="slider-fill" id="sliderFill"></div>
<div class="slider-thumb" id="sliderThumb"></div>
</div>
<div class="slider-labels">
<span>Muy cerca</span><span>Cerca</span><span>Medio</span><span>Lejos</span><span>Sin señal</span>
</div>
</div>
</div>

<div class="stats-card card">
<div class="stat"><span class="stat-label">Clientes</span><span class="stat-value" id="clientCount">0</span></div>
<div class="stat"><span class="stat-label">Activo</span><span class="stat-value" id="uptimeDisplay">00:00:00</span></div>
</div>

<button class="arm-btn" id="armBtn">ARMAR ALARMA</button>

<div class="footer">
<span>IP: 192.168.4.1</span> &middot; <span>Próxima: <span id="countdown">10</span>s</span>
<p class="version">v1.0 — Pulsar IoT</p>
</div>
</div>

<script>
(function(){
var E={dot:document.getElementById('statusDot'),label:document.getElementById('distanceLabel'),fill:document.getElementById('signalFill'),rssi:document.getElementById('rssiValue'),distText:document.getElementById('distanceText'),sFill:document.getElementById('sliderFill'),sThumb:document.getElementById('sliderThumb'),clients:document.getElementById('clientCount'),uptime:document.getElementById('uptimeDisplay'),cd:document.getElementById('countdown'),btn:document.getElementById('armBtn')};
var countdown=0,armed=false;

E.btn.addEventListener('click',function(){armed=!armed;E.btn.textContent=armed?'DESARMAR ALARMA':'ARMAR ALARMA';E.btn.className='arm-btn'+(armed?' armed':'');});

function formatUptime(s){var h=Math.floor(s/3600);var m=Math.floor((s%3600)/60);var se=s%60;return String(h).padStart(2,'0')+':'+String(m).padStart(2,'0')+':'+String(se).padStart(2,'0');}

function rssiToPct(r){if(r>=-30)return 100;if(r<=-100)return 0;return Math.round((r+100)*100/70);}

function rssiToSlider(r){if(r>=-30)return 95;if(r<=-100)return 5;return 5+((r+100)*90/70);}

function fetchStatus(){
fetch('/api/status').then(function(r){return r.json();}).then(function(d){
var rssi=d.rssi,cl=d.clients,pct=rssiToPct(rssi),sp=rssiToSlider(rssi);
E.clients.textContent=cl;
E.uptime.textContent=formatUptime(d.uptime);
if(cl===0){
E.dot.className='status-dot';
E.label.textContent='Sin conexion';
E.rssi.textContent='-- dBm';
E.distText.textContent='Sin conexion';
E.fill.style.width='0%';
E.sFill.style.width='0%';
E.sThumb.style.left='5%';
}else{
E.label.textContent=d.distance;
E.rssi.textContent=rssi+' dBm';
E.distText.textContent=d.distance;
if(rssi>=-50){E.dot.className='status-dot connected';
}else if(rssi>=-75){E.dot.className='status-dot fair';
}else{E.dot.className='status-dot poor';}
E.fill.style.width=pct+'%';
E.sFill.style.width=sp+'%';
E.sThumb.style.left=sp+'%';
}
countdown=10;
E.cd.textContent=10;
}).catch(function(){
E.dot.className='status-dot';
E.label.textContent='Error de conexion';
E.rssi.textContent='--';
E.distText.textContent='--';
E.fill.style.width='0%';
E.sFill.style.width='0%';
E.sThumb.style.left='5%';
E.clients.textContent='0';
});
}

setInterval(function(){if(countdown>0){countdown--;}E.cd.textContent=countdown;if(countdown===0){fetchStatus();}},1000);

fetchStatus();
})();
</script>
</body>
</html>
)rawhtml";
