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
.card{background:#fff;border-radius:16px;padding:20px;margin-bottom:16px;box-shadow:0 1px 3px rgba(0,0,0,.06),0 1px 2px rgba(0,0,0,.04)}
.status-card{text-align:center}
.status-indicator{display:inline-flex;align-items:center;gap:10px;background:#f8fafc;padding:10px 24px;border-radius:40px;margin-bottom:16px}
.status-dot{width:14px;height:14px;border-radius:50%;background:#94a3b8;transition:background .4s,box-shadow .4s}
.status-dot.connected{background:#10b981;box-shadow:0 0 8px rgba(16,185,129,.5)}
.status-dot.fair{background:#f59e0b;box-shadow:0 0 8px rgba(245,158,11,.5)}
.status-dot.poor{background:#ef4444;box-shadow:0 0 8px rgba(239,68,68,.5)}
#distanceLabel{font-size:1.1rem;font-weight:600}
.signal-bar-container{display:flex;align-items:center;gap:12px;margin-bottom:8px}
.signal-bar{flex:1;height:28px;background:#e2e8f0;border-radius:14px;overflow:hidden;position:relative}
.signal-fill{height:100%;border-radius:14px;background:linear-gradient(90deg,#ef4444,#f59e0b,#10b981);transition:width .6s ease;width:0%}
.signal-value{font-size:1rem;font-weight:700;color:#0f172a;white-space:nowrap;min-width:60px}
.distance-text{font-size:.9rem;color:#64748b;margin-top:4px}
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
.chart-card{padding:20px}
.chart-card h3{font-size:.85rem;color:#64748b;margin-bottom:12px;text-transform:uppercase;letter-spacing:.5px}
.chart-wrap{width:100%;overflow:hidden}
#chart{width:100%;height:220px;display:block}
.chart-legend{display:flex;gap:20px;justify-content:center;margin-top:10px;font-size:.8rem;color:#64748b}
.chart-legend span{display:flex;align-items:center;gap:6px}
.chart-legend i{display:inline-block;width:20px;height:3px;border-radius:2px}
.chart-legend .l1 i{background:#2563eb}
.chart-legend .l2 i{background:#10b981}
.history-count{text-align:center;font-size:.75rem;color:#94a3b8;margin-top:8px}
.footer{text-align:center;padding:8px 0 24px;color:#94a3b8;font-size:.8rem;line-height:1.8}
.footer .version{font-size:.7rem;color:#cbd5e1;margin-top:2px}
.conn-banner{display:none;background:#ef4444;color:#fff;text-align:center;padding:12px 16px;border-radius:14px;margin-bottom:16px;font-size:.85rem;line-height:1.4;animation:fadeIn .3s ease;box-shadow:0 2px 8px rgba(239,68,68,.3)}
.conn-banner span{font-size:1.1rem}
.conn-banner.show{display:block}
.stale{opacity:.4;pointer-events:none;transition:opacity .4s}
.stale .status-dot{background:#94a3b8!important;box-shadow:none!important}
@keyframes fadeIn{from{opacity:0;transform:translateY(-8px)}to{opacity:1;transform:translateY(0)}}
@media(max-width:400px){.card{padding:16px}.status-indicator{padding:8px 18px}.stat-value{font-size:1.1rem}#chart{height:180px}}
</style>
</head>
<body>
<div class="container">
<header>
<h1>PULSAR 180 GT</h1>
<p>Sistema de Proximidad</p>
</header>

<div class="conn-banner" id="connBanner"><span>&#9888;</span> SIN CONEXI&Oacute;N &mdash; Verifica que tu tel&eacute;fono est&eacute; conectado a <strong>Pulsar-180</strong></div>

<div class="card status-card">
<div class="status-indicator">
<div class="status-dot" id="statusDot"></div>
<span id="distanceLabel">Conectando...</span>
</div>
<div class="signal-bar-container">
<div class="signal-bar"><div class="signal-fill" id="signalFill"></div></div>
<span class="signal-value" id="rssiValue">--</span>
</div>
<p class="distance-text">Distancia: <span id="distanceText">--</span></p>
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

<div class="card chart-card">
<h3>Historial de Se&ntilde;al</h3>
<div class="chart-wrap">
<canvas id="chart"></canvas>
</div>
<div class="chart-legend">
<span class="l1"><i></i> RSSI (dBm)</span>
<span class="l2"><i></i> Distancia (m)</span>
</div>
<div class="history-count" id="historyCount"></div>
</div>

<div class="stats-card card">
<div class="stat"><span class="stat-label">Clientes</span><span class="stat-value" id="clientCount">0</span></div>
<div class="stat"><span class="stat-label">Activo</span><span class="stat-value" id="uptimeDisplay">00:00:00</span></div>
</div>

<button class="arm-btn" id="armBtn">ARMAR ALARMA</button>

<div class="footer">
<span>IP: 192.168.4.1</span> &middot; <span>Pr&oacute;xima: <span id="countdown">10</span>s</span>
<p class="version">v1.1 — Pulsar IoT</p>
</div>
</div>

<script>
(function(){
var E={dot:document.getElementById('statusDot'),label:document.getElementById('distanceLabel'),fill:document.getElementById('signalFill'),rssi:document.getElementById('rssiValue'),distText:document.getElementById('distanceText'),sFill:document.getElementById('sliderFill'),sThumb:document.getElementById('sliderThumb'),clients:document.getElementById('clientCount'),uptime:document.getElementById('uptimeDisplay'),cd:document.getElementById('countdown'),btn:document.getElementById('armBtn'),histCnt:document.getElementById('historyCount'),banner:document.getElementById('connBanner')};
var canvas=document.getElementById('chart'),ctx=canvas&&canvas.getContext('2d');
var countdown=0,armed=false,history=[],MAX_H=500,lastDataTime=0,STALE_T=20000,stale=false;

function setStale(){if(stale)return;stale=true;E.banner.classList.add('show');document.querySelectorAll('.card').forEach(function(c){c.classList.add('stale')});E.btn.style.opacity='.4';E.dot.className='status-dot'}
function clearStale(){if(!stale)return;stale=false;E.banner.classList.remove('show');document.querySelectorAll('.card').forEach(function(c){c.classList.remove('stale')});E.btn.style.opacity='1'}

function saveH(){try{if(history.length>MAX_H)history=history.slice(-MAX_H);localStorage.setItem('pulsar_history',JSON.stringify(history))}catch(e){}}
function loadH(){try{var r=localStorage.getItem('pulsar_history');if(r)history=JSON.parse(r)}catch(e){}if(!Array.isArray(history))history=[]}
function addP(rssi,meters){
var n=new Date(),h=n.getHours(),m=n.getMinutes(),s=n.getSeconds();
var t=(h<10?'0':'')+h+':'+(m<10?'0':'')+m+':'+(s<10?'0':'')+s;
var d=n.getFullYear()+'-'+((n.getMonth()+1)<10?'0':'')+(n.getMonth()+1)+'-'+(n.getDate()<10?'0':'')+n.getDate();
history.push({t:t,d:d,r:rssi,m:meters});saveH();
}

function fmtUp(s){var h=Math.floor(s/3600),m=Math.floor((s%3600)/60),se=s%60;return(h<10?'0':'')+h+':'+(m<10?'0':'')+m+':'+(se<10?'0':'')+se}
function r2p(r){if(r>=-30)return 100;if(r<=-100)return 0;return Math.round((r+100)*100/70)}
function r2s(r){if(r>=-30)return 95;if(r<=-100)return 5;return 5+((r+100)*90/70)}

function drawChart(){
if(!ctx||!canvas)return;
var W=canvas.width,H=canvas.height,pt=15,pb=22,pl=38,pr=38,pw=W-pl-pr,ph=H-pt-pb;
ctx.clearRect(0,0,W,H);
var pts=history.slice(-60),N=pts.length;
if(N<2){ctx.fillStyle='#94a3b8';ctx.font='14px sans-serif';ctx.textAlign='center';ctx.fillText('Esperando datos...',W/2,H/2+5);return}
var grd=[-30,-40,-50,-60,-70,-80,-90,-100];
ctx.strokeStyle='#e2e8f0';ctx.lineWidth=1;ctx.font='10px sans-serif';
for(var i=0;i<grd.length;i++){
var y=pt+((-30-grd[i])/70)*ph;
ctx.beginPath();ctx.moveTo(pl,y);ctx.lineTo(W-pr,y);ctx.stroke();
ctx.fillStyle='#64748b';ctx.textAlign='right';ctx.fillText(grd[i],pl-4,y+3);
var d=Math.pow(10,(grd[i]+40)/-25);if(d>99){d=Math.round(d);ctx.fillStyle='#94a3b8';ctx.textAlign='left';ctx.fillText(d+'m',W-pr+4,y+3);continue;}
ctx.fillStyle='#94a3b8';ctx.textAlign='left';ctx.fillText((d<10?d.toFixed(1):Math.round(d))+'m',W-pr+4,y+3);
}
ctx.strokeStyle='#e2e8f0';ctx.beginPath();ctx.moveTo(pl,pt);ctx.lineTo(pl,pt+ph);ctx.lineTo(W-pr,pt+ph);ctx.stroke();
// RSSI line (blue)
ctx.strokeStyle='#2563eb';ctx.lineWidth=2;ctx.lineJoin='round';
ctx.beginPath();
for(var i=0;i<N;i++){
var x=pl+(i/(N-1))*pw;
var r=pts[i].r;if(r<-100)r=-100;if(r>-30)r=-30;
var y=pt+((-30-r)/70)*ph;
i===0?ctx.moveTo(x,y):ctx.lineTo(x,y);
}
ctx.stroke();
// Distance line (green)
ctx.strokeStyle='#10b981';ctx.lineWidth=2;ctx.lineJoin='round';
ctx.beginPath();
for(var i=0;i<N;i++){
var x=pl+(i/(N-1))*pw;
var m=pts[i].m;if(m<0)m=0;if(m>50){m=50;}
var y=pt+((50-m)/50)*ph;
i===0?ctx.moveTo(x,y):ctx.lineTo(x,y);
}
ctx.stroke();
// X time labels
if(N>1){
ctx.fillStyle='#94a3b8';ctx.font='9px sans-serif';ctx.textAlign='center';
var step=Math.max(1,Math.floor(N/5));
for(var i=0;i<N;i+=step){
var x=pl+(i/(N-1))*pw;
ctx.fillText(pts[i].t,x,pt+ph+15);
}
}
}

function resizeC(){if(!canvas)return;var r=canvas.parentElement.getBoundingClientRect();canvas.width=r.width;canvas.height=220;drawChart()}

E.btn.addEventListener('click',function(){armed=!armed;E.btn.textContent=armed?'DESARMAR ALARMA':'ARMAR ALARMA';E.btn.className='arm-btn'+(armed?' armed':'');});

function fetchStatus(){
fetch('/api/status').then(function(r){return r.json();}).then(function(d){
clearStale();lastDataTime=Date.now();
var rssi=d.rssi,cl=d.clients,pct=r2p(rssi),sp=r2s(rssi),dm=d.distance_meters;
E.clients.textContent=cl;
E.uptime.textContent=fmtUp(d.uptime);
if(cl===0){
E.dot.className='status-dot';
E.label.textContent='Sin conexion';
E.rssi.textContent='-- dBm';
E.distText.textContent='Sin conexion';
E.fill.style.width='0%';E.sFill.style.width='0%';E.sThumb.style.left='5%';
}else{
E.label.textContent=d.distance;
E.rssi.textContent=rssi+' dBm';
E.distText.textContent=(dm<0?'--':(dm<10?dm.toFixed(1):Math.round(dm)))+' m ('+d.distance+')';
if(rssi>=-50){E.dot.className='status-dot connected';
}else if(rssi>=-75){E.dot.className='status-dot fair';
}else{E.dot.className='status-dot poor';}
E.fill.style.width=pct+'%';E.sFill.style.width=sp+'%';E.sThumb.style.left=sp+'%';
addP(rssi,dm<0?0:dm);drawChart();
E.histCnt.textContent=history.length+' registros guardados';
}
countdown=10;E.cd.textContent=10;
}).catch(function(){
setStale();countdown=3;E.cd.textContent=3;
E.label.textContent='Sin conexion';
E.rssi.textContent='--';
E.distText.textContent='--';
E.fill.style.width='0%';E.sFill.style.width='0%';E.sThumb.style.left='5%';E.clients.textContent='0';
});
}

loadH();if(E.histCnt)E.histCnt.textContent=history.length+' registros guardados';
resizeC();window.addEventListener('resize',resizeC);
setInterval(function(){if(lastDataTime!==0&&Date.now()-lastDataTime>STALE_T)setStale();if(countdown>0)countdown--;E.cd.textContent=countdown;if(countdown===0)fetchStatus()},1000);
fetchStatus();
})();
</script>
</body>
</html>
)rawhtml";
