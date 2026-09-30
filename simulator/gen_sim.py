#!/usr/bin/env python3
"""Generate the nightscout-clock face simulator as a single self-contained HTML file."""
import os
import json

HERE = os.path.dirname(os.path.abspath(__file__))
AW = json.load(open(os.path.join(HERE, 'fonts', 'font_awtrix.json')))
MU = json.load(open(os.path.join(HERE, 'fonts', 'font_mu.json')))
SMILEY = json.load(open(os.path.join(HERE, 'fonts', 'smiley_bitmaps.json')))

# ---- static data transcribed from firmware source ----
ARROWS = {
    'DOUBLE_UP':    [0b01010000,0b11111000,0b01010000,0b01010000,0b01010000],
    'SINGLE_UP':    [0b00100000,0b01110000,0b10101000,0b00100000,0b00100000],
    'FORTYFIVE_UP': [0b00111000,0b00011000,0b00101000,0b01000000,0b10000000],
    'FLAT':         [0b00100000,0b00010000,0b11111000,0b00010000,0b00100000],
    'FORTYFIVE_DOWN':[0b10000000,0b01000000,0b00101000,0b00011000,0b00111000],
    'SINGLE_DOWN':  [0b00100000,0b00100000,0b10101000,0b01110000,0b00100000],
    'DOUBLE_DOWN':  [0b01010000,0b01010000,0b01010000,0b11111000,0b01010000],
    'NONE':         [0,0,0,0,0],
    'OLD':          [0b10001000,0b01010000,0b00100000,0b01010000,0b10001000],
}
SPARK = {
    'A': [0b00111100,0b01000010,0b10100101,0b10000001,0b10100101,0b10011001,0b01000010,0b00111100],
    'B': [0b00111100,0b01000010,0b10100101,0b10000001,0b10000001,0b10111101,0b01000010,0b00111100],
    'S': [0b00111100,0b01000010,0b10100101,0b10100101,0b10000001,0b10011001,0b10011001,0b00111100],
    'D': [0b00111100,0b01000010,0b10011001,0b00100100,0b10000001,0b01011010,0b01000010,0b00111100],
}
# Upstream unicorn sprite (12x8, palette indices), palettes in RGB565
UNICORN_SPRITE = [
    1,1,0,0,0,0,0,0,0,0,0,0,
    0,1,1,0,0,4,5,6,7,0,0,0,
    0,0,1,1,4,4,4,5,6,7,0,0,
    0,0,0,2,2,2,2,4,5,6,7,0,
    2,2,2,2,3,2,2,4,5,6,7,0,
    2,2,2,2,2,2,2,4,5,6,7,0,
    0,2,2,2,2,2,2,4,5,6,7,0,
    0,0,0,2,2,2,2,2,4,5,6,7,
]
PAL_N = [0xFE87,0xF79D,0x18C3,0x4D5F,0x5EAD,0xFCC7,0xFA78,0x9AFE]
# Critter sprites (13x8, palette indices 0-3). 1=body (level color), 2=accent, 3=eye.
# NOTE: simulator-only experiment — firmware still uses 12x8 sprites.
CRITTERS = {
 'Cat':{'sprite':[0,1,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,3,1,1,3,1,0,0,0,0,0,0,0,1,1,1,2,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,0,0,1,1,1,1,1,1,0,0,0,1,1,0,0,1,1,1,1,1,1,1,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0],'body':0xFC00,'accent':0xFFFF,'eye':0x0000},
 'Dog':{'sprite':[0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,3,1,1,1,3,1,0,0,0,0,0,0,1,1,1,2,2,1,1,0,0,0,0,1,0,0,1,1,2,2,1,1,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,2,1,1,1,1,1,1,2,0,0,0],'body':0xD343,'accent':0xFD20,'eye':0x0000},
 'Frog':{'sprite':[0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,1,3,1,1,3,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,2,2,2,2,1,1,0,0,0,0,0,1,1,1,2,2,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0],'body':0x07E0,'accent':0x87F0,'eye':0x0000},
 'Fox':{'sprite':[0,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,3,1,1,1,1,1,1,3,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,2,2,1,1,1,0,0,0,0,0,0,1,1,2,2,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0],'body':0xFC00,'accent':0xFFFF,'eye':0x0000},
 'Bunny':{'sprite':[0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,1,2,0,1,2,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,3,1,3,1,0,0,0,0,0,0,0,0,1,1,2,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0],'body':0xFFFF,'accent':0xF81F,'eye':0x0000},
 'Narwhal':{'sprite':[0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,1,1,0,2,0,0,0,0,0,0,0,0,1,1,3,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,0,0,1,1,2,1,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0],'body':0x94B2,'accent':0xFFFF,'eye':0x0000},
 'Whale':{'sprite':[0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,3,1,1,1,3,1,1,1,1,1,0,1,1,1,1,1,1,1,2,1,1,1,1,0,0,1,1,2,2,2,2,1,1,0,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,0,0,0,0,1,1,1,1,0,0,0,0,0],'body':0x439F,'accent':0xFFFF,'eye':0x0000},
 'Mario':{'sprite':[0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,3,3,1,1,3,3,1,1,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,2,3,2,2,2,2,2,3,2,0,0,0,0,2,2,3,3,3,3,2,2,0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0],'body':0xF800,'accent':0xFD20,'eye':0x0000},
 'Luigi':{'sprite':[0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,3,3,1,1,3,3,1,1,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,2,3,2,2,2,2,2,3,2,0,0,0,0,2,2,3,3,3,3,2,2,0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0],'body':0x07E0,'accent':0xFD20,'eye':0x0000},
 'Peach':{'sprite':[0,0,2,0,2,0,2,0,2,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,3,3,3,3,3,3,3,3,3,3,0,0,0,3,3,1,1,1,1,1,1,3,3,0,0,0,3,1,1,0,1,1,0,1,1,3,0,0,0,3,1,1,1,1,1,1,1,1,3,0,0,0,2,2,1,1,0,0,1,1,2,2,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0],'body':0xFD20,'accent':0xFCF4,'eye':0xFFE0},
 'Toad':{'sprite':[0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,2,2,1,1,2,2,1,0,0,0,0,1,1,2,2,1,1,2,2,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,2,2,2,2,2,2,2,0,0,0,0,0,0,2,3,2,2,2,3,2,0,0,0,0,0,0,2,2,2,2,2,2,2,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0],'body':0xF800,'accent':0xFFFF,'eye':0x0000},
 'Pumpkin':{'sprite':[0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,3,1,1,3,3,1,1,3,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,3,3,3,3,3,3,1,1,0,0,0,1,1,1,3,1,1,3,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0],'body':0xFC00,'accent':0x07E0,'eye':0x0000},
 'Ghost':{'sprite':[0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,3,1,1,1,1,3,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,3,3,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0],'body':0xFFFF,'accent':0xFFFF,'eye':0x0000},
 'Witch':{'sprite':[0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,2,2,2,2,2,2,2,0,0,0,0,0,0,2,3,2,2,2,3,2,0,0,0,0,0,0,2,2,2,2,2,2,2,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0],'body':0xA81F,'accent':0x07E0,'eye':0x0000},
 'Turkey':{'sprite':[0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,3,1,3,1,0,0,0,0,0,0,0,0,1,1,2,2,1,0,0,0,0,0,0,0,0,1,1,2,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,4,5,6,4,5,6,4,5,6,4,0,0,4,4,5,5,6,6,6,5,5,4,4,0,0],'body':0xD343,'accent':0xF800,'eye':0x0000,'c4':0xF800,'c5':0xFC00,'c6':0xFFE0},
 'Butterfly':{'sprite':[0,1,1,1,0,0,0,1,1,1,0,0,0,0,1,1,1,1,2,1,1,1,1,0,0,0,0,1,1,1,1,2,1,1,1,1,0,0,0,0,0,1,1,1,2,1,1,1,0,0,0,0,0,0,1,1,1,2,1,1,1,0,0,0,0,0,1,1,1,1,2,1,1,1,1,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],'body':0xFC9F,'accent':0x8410,'eye':0x0000},
}


JS_DATA = "const AW=%s;\nconst MU=%s;\nconst SMILEY=%s;\nconst ARROWS=%s;\nconst SPARK=%s;\nconst USPRITE=%s;\nconst UPALN=%s;\nconst CRITTERS=%s;\n" % (
    json.dumps(AW, separators=(',',':')),
    json.dumps(MU, separators=(',',':')),
    json.dumps(SMILEY, separators=(',',':')),
    json.dumps(ARROWS, separators=(',',':')),
    json.dumps(SPARK, separators=(',',':')),
    json.dumps(UNICORN_SPRITE, separators=(',',':')),
    json.dumps(PAL_N, separators=(',',':')),
    json.dumps(CRITTERS, separators=(',',':')),
)

JS_ENGINE = r"""
// ================= engine =================
const C={BLACK:0,BLUE:0x001F,GREEN:0x07E0,CYAN:0x07FF,GRAY:0xA514,RED:0xF800,MAGENTA:0xF81F,YELLOW:0xFFE0,WHITE:0xFFFF};
const S={value:142,trend:'FLAT',ageMin:3,noData:false,mmol:false,h12:false,stale:0xA514,battery:87};
const cssCache={};
function css(c){c|=0;let s=cssCache[c];if(s)return s;
  const r=(c>>11)&31,g=(c>>5)&63,b=c&31;
  s=`rgb(${(r*255/31)|0},${(g*255/63)|0},${(b*255/31)|0})`;cssCache[c]=s;return s;}
function rgb565(r,g,b){return (((r&0xF8)<<8)|((g&0xFC)<<3)|(b>>3))|0;}
function hsv(h){h&=255;const region=(h/43)|0,rem=(h-region*43)*6,q=255-rem,t=rem;
  switch(region){case 0:return rgb565(255,t,0);case 1:return rgb565(q,255,0);case 2:return rgb565(0,255,t);
  case 3:return rgb565(0,q,255);case 4:return rgb565(t,0,255);default:return rgb565(255,0,q);}}
function fade(c){let r=(c>>11)&31,g=(c>>5)&63,b=c&31;
  r=(r*4/5)|0;g=(g*4/5)|0;b=(b*4/5)|0;return (r<<11)|(g<<5)|b;}
function px(b,x,y,c){if(x>=0&&x<32&&y>=0&&y<8)b[y*32+x]=c|0;}
function bitmap(b,x,y,bytes,w,h,color){
  // Adafruit GFX drawBitmap semantics: one byte per row, MSB = leftmost pixel.
  for(let r=0;r<h;r++)for(let c=0;c<w;c++){
    if((bytes[r]>>(7-c))&1)px(b,x+c,y+r,color);}}
function drawIndexedSprite(b,x,y,sprite,w,h,palette){
  for(let r=0;r<h;r++)for(let c=0;c<w;c++){
    const idx=sprite[r*w+c];if(idx>0)px(b,x+c,y+r,palette[idx]);}}
function critterPalette(name,level,old){
  const c=CRITTERS[name];let body=c.body,accent=c.accent;
  if(old){body=S.stale;accent=S.stale;}
  else if(level==='WL'||level==='UL')body=C.RED;
  else if(level==='WH'||level==='UH')body=C.YELLOW;
  const e4=c.c4!==undefined?c.c4:accent,e5=c.c5!==undefined?c.c5:accent,e6=c.c6!==undefined?c.c6:accent,e7=c.c7!==undefined?c.c7:accent;
  return[0,body,accent,c.eye,e4,e5,e6,e7];}
function critterFace(b,name){
  const last=READINGS[READINGS.length-1],old=isOld(),c=CRITTERS[name];
  if(!last){drawIndexedSprite(b,0,0,c.sprite,13,8,critterPalette(name,null,true));drawText(b,AW,noDataText(),33,6,1,S.stale);return;}
  drawIndexedSprite(b,0,0,c.sprite,13,8,critterPalette(name,level(last.sgv),old));
  trendArrow(b,last,14,2,old,false);
  drawText(b,AW,printable(last.sgv),32,6,1,old?S.stale:levelColor(last.sgv));
  timerBlocks(b,last,16,16,7);}
// ---- text: Adafruit GFX custom-font semantics, pixel exact ----
function charW(F,ch){const w=F.charMap[ch.charCodeAt(0)];return w===undefined?4:w;}
function textW(F,s){let w=0;for(const ch of s)w+=charW(F,ch);return w;}
function drawText(b,F,s,x,y,align,color){ // align: 0 LEFT, 1 RIGHT, 2 CENTER
  const w=textW(F,s);
  let cx=align===0?x:align===1?x-w:Math.trunc((32-w)/2);
  for(const ch of s){
    const gi=ch.charCodeAt(0)-F.first;
    if(gi>=0&&gi<F.glyphs.length){
      const g=F.glyphs[gi],off=g[0],gw=g[1],gh=g[2],adv=g[3],xo=g[4],yo=g[5];
      for(let r=0;r<gh;r++)for(let c=0;c<gw;c++){const i=r*gw+c;
        if((F.bitmaps[off+(i>>3)]>>(7-(i&7)))&1)px(b,cx+xo+c,y+yo+r,color);}
      cx+=adv;
    } else cx+=4;
  }
}
// ---- glucose model ----
function level(sgv){if(sgv<55)return 'UL';if(sgv<70)return 'WL';if(sgv<=180)return 'N';if(sgv<250)return 'WH';return 'UH';}
function levelColor(sgv){const l=level(sgv);return l==='N'?C.GREEN:((l==='WL'||l==='WH')?C.YELLOW:C.RED);}
function printable(sgv){return S.mmol?(sgv/18).toFixed(1):String(sgv);}
let READINGS=[];
function buildReadings(){
  if(S.noData){READINGS=[];return;}
  const per5={DOUBLE_UP:12,SINGLE_UP:6,FORTYFIVE_UP:3,FLAT:0,FORTYFIVE_DOWN:-3,SINGLE_DOWN:-6,DOUBLE_DOWN:-12,NONE:0}[S.trend];
  const arr=[];let v=S.value;
  for(let i=0;i<36;i++){
    arr.unshift({sgv:Math.round(v),ageSec:S.ageMin*60+i*300,trend:i===0?S.trend:'FLAT'});
    v-=per5;v+=Math.sin(i*1.7)*1.5;v=Math.max(40,Math.min(400,v));
  }
  READINGS=arr;
}
function isOld(){const l=READINGS[READINGS.length-1];return l?l.ageSec>=20*60:false;}
function trendArrow(b,rd,x,y,old,cbr){
  if(old){bitmap(b,x,y,ARROWS.OLD,5,5,S.stale);return;}
  bitmap(b,x,y,ARROWS[rd.trend]||ARROWS.NONE,5,5,cbr?levelColor(rd.sgv):C.WHITE);
}
function c565(c){return [((c>>11)&31)*255/31,((c>>5)&63)*255/63,(c&31)*255/31];}
function blend(a,b2,amt){const A=c565(a),B=c565(b2),k=255-amt;
  return rgb565((A[0]*k+B[0]*amt)/255,(A[1]*k+B[1]*amt)/255,(A[2]*k+B[2]*amt)/255);}
function trendVLine(b,x,trend,old){
  if(old)trend='NONE';
  const P=(yy,c)=>px(b,x,yy,c);
  switch(trend){
    case 'DOUBLE_UP':P(0,C.RED);P(1,C.YELLOW);P(2,C.GREEN);P(3,C.WHITE);break;
    case 'DOUBLE_DOWN':P(4,C.WHITE);P(5,C.GREEN);P(6,C.YELLOW);P(7,C.RED);break;
    case 'SINGLE_UP':P(1,C.YELLOW);P(2,C.GREEN);P(3,C.WHITE);break;
    case 'SINGLE_DOWN':P(4,C.WHITE);P(5,C.GREEN);P(6,C.YELLOW);break;
    case 'FORTYFIVE_UP':P(2,C.GREEN);P(3,C.WHITE);break;
    case 'FORTYFIVE_DOWN':P(4,C.WHITE);P(5,C.GREEN);break;
    case 'FLAT':P(3,C.WHITE);P(4,C.WHITE);break;
  }
}
function timerBlocks(b,last,width,xPos,yPos){
  const MAX=5;let n=(last.ageSec/60)|0;if(n>MAX)n=MAX;if(n<=0)return;
  const bs=((width-4)/MAX)|0;if(bs<1)return;
  xPos+=Math.trunc((width-(bs*MAX+(MAX-1)))/2);
  let col=C.GREEN;
  if(last.ageSec>=60*20)col=S.stale;else if(last.ageSec>=(MAX+1)*60)col=C.YELLOW;
  for(let i=0;i<n;i++)for(let j=0;j<bs;j++)px(b,xPos+i*(bs+1)+j,yPos,col);
}
function graph(b,x0,len,forMin){
  const ps=(forMin*60)/len;
  for(let i=0;i<len;i++){
    let sum=0,n=0;
    for(const r of READINGS){if(r.ageSec>=i*ps&&r.ageSec<(i+1)*ps){sum+=r.sgv;n++;}}
    if(!n)continue;
    const avg=sum/n,l=level(avg);let yy,col;
    if(l==='UH'){yy=0;col=C.RED;}else if(l==='WH'){yy=1;col=C.YELLOW;}
    else if(l==='N'){yy=5-Math.trunc((avg-70)*4/(180-70));col=C.GREEN;}
    else if(l==='WL'){yy=6;col=C.YELLOW;}else{yy=7;col=C.RED;}
    px(b,x0+len-1-i,yy,col);
  }
}
function noDataText(){return S.mmol?"--.-":"---";}
"""

JS_FACES = r"""
// ================= faces =================
const FACES=[
{name:"Simple",draw(b,t){
  const last=READINGS[READINGS.length-1],old=isOld();
  if(!last){drawText(b,AW,noDataText(),0,6,2,S.stale);return;}
  drawText(b,AW,printable(last.sgv),0,6,2,old?S.stale:levelColor(last.sgv));
  trendArrow(b,last,27,1,old);
  timerBlocks(b,last,32,0,7);
}},
{name:"Full graph",draw(b,t){graph(b,0,32,180);}},
{name:"Graph and BG",draw(b,t){
  const last=READINGS[READINGS.length-1],old=isOld();
  if(!last){drawText(b,AW,noDataText(),30,6,1,S.stale);return;}
  const p=printable(last.sgv),tw=textW(AW,p),gw=32-tw-2;
  graph(b,0,gw,gw*5);
  drawText(b,AW,p,30,6,1,old?S.stale:levelColor(last.sgv));
  trendVLine(b,31,last.trend,old);
  timerBlocks(b,last,tw+2,gw,7);
}},
{name:"Big text",draw(b,t){
  const last=READINGS[READINGS.length-1],old=isOld();
  const s=last?printable(last.sgv):noDataText();
  drawText(b,MU,s,0,7,0,!last||old?S.stale:levelColor(last.sgv));
  if(last)trendArrow(b,last,27,1,old);
}},
{name:"Value and diff",draw(b,t){
  const last=READINGS[READINGS.length-1],old=isOld();
  if(!last){drawText(b,AW,noDataText(),13,6,1,S.stale);return;}
  const x=(S.mmol&&last.sgv>=180)?14:13;
  drawText(b,AW,printable(last.sgv),x,6,1,old?S.stale:levelColor(last.sgv));
  trendArrow(b,last,13,1,old);
  let diff="?";
  if(READINGS.length>=2){const d=last.sgv-READINGS[READINGS.length-2].sgv;
    diff=Math.abs(d)>99?"?":((d>=0?"+":"")+printable(d));}
  drawText(b,AW,diff,33,6,1,old?S.stale:C.WHITE);
  timerBlocks(b,last,32,0,7);
}},
{name:"Clock",draw(b,t){
  const now=new Date();let h=now.getHours();const m=now.getMinutes();
  if(S.h12){const pm=h>=12;h=h%12===0?12:h%12;for(let i=0;i<16;i++)px(b,i,7,pm?C.BLUE:C.CYAN);}
  drawText(b,AW,String(h).padStart(2,'0'),0,6,0,C.WHITE);
  drawText(b,AW,String(m).padStart(2,'0'),9,6,0,C.WHITE);
  const last=READINGS[READINGS.length-1],old=isOld();
  if(last){drawText(b,AW,printable(last.sgv),30,6,1,old?S.stale:levelColor(last.sgv));
    if(S.h12)timerBlocks(b,last,15,18,7);else timerBlocks(b,last,32,0,7);
    trendVLine(b,31,last.trend,old);
  }else drawText(b,AW,noDataText(),33,6,1,S.stale);
}},
{name:"Diagnostics",draw(b,t){
  // Mirrors BGDisplayFaceDiagnostics: "SAT 26/09 21:45" (small font, cyan) +
  // reading (medium font, level color / stale) + trend arrow; scrolls when wider than 32px.
  const last=READINGS[READINGS.length-1],old=isOld();
  const now=new Date(),p2=n=>String(n).padStart(2,'0');
  const wd=["SUN","MON","TUE","WED","THU","FRI","SAT"][now.getDay()];
  const dt=`${wd} ${p2(now.getDate())}/${p2(now.getMonth()+1)} ${p2(now.getHours())}:${p2(now.getMinutes())}`;
  const dtW=textW(AW,dt);
  let w,drawAt;
  if(!last){
    const nd=noDataText(),ndW=textW(AW,nd);
    w=dtW+2+ndW;
    drawAt=sx=>{drawText(b,AW,dt,sx,7,0,C.CYAN);drawText(b,AW,nd,sx+dtW+2,7,0,S.stale);};
  }else{
    const rs=printable(last.sgv),rW=textW(AW,rs),rX=dtW+2,aX=rX+rW+1;
    w=aX+5;
    const rc=old?S.stale:levelColor(last.sgv);
    drawAt=sx=>{drawText(b,AW,dt,sx,7,0,C.CYAN);
      drawText(b,AW,rs,sx+rX,7,0,rc);
      trendArrow(b,last,sx+aX,2,old,true);};
  }
  let sx;
  if(w>32){const travel=w+32+2;sx=Math.trunc(32-(((t/33)*0.37)%travel));}
  else sx=Math.trunc((32-w)/2);
  drawAt(sx);
}},
{name:"Battery and uptime",draw(b,t){
  const dots=S.battery===100?10:((S.battery/10)|0);
  const bc=S.battery<10?C.RED:S.battery<30?C.YELLOW:C.GREEN;
  for(let i=0;i<10;i++)px(b,11+i,0,i<dots?bc:C.GRAY);
  drawText(b,AW,S.battery+"%",0,7,0,bc);
  const up=(t/1000)|0,um=(up/60)|0;let ut;
  if(um<100)ut=um+"M";else{const uh=(um/60)|0;ut=uh<100?uh+"H":(((uh/24)|0)+"D");}
  drawText(b,AW,ut,31,7,1,C.WHITE);
}},
{name:"Rainbow big text",draw(b,t){
  // Mirrors BGDisplayFaceBigTextRainbow: 2Hz blink when stale; per-character
  // hue blended 192/255 over the level color; trend arrow colored by reading.
  const last=READINGS[READINGS.length-1],old=isOld();
  if(old&&(((t/500)|0)%2===1))return;
  const s=last?printable(last.sgv):noDataText(),n=s.length;
  const base=!last||old?S.stale:levelColor(last.sgv),h0=((t/20)|0)&255;
  let x=0;
  for(let i=0;i<n;i++){
    const hue=(((i*255)/Math.max(n,1))|0)+h0;
    drawText(b,MU,s[i],x,7,0,(!last||old)?base:blend(base,hsv(hue),192));
    x+=textW(MU,s[i]);
  }
  if(last)trendArrow(b,last,27,1,old,true);
}},
{name:"Smiley",draw(b,t){
  const last=READINGS[READINGS.length-1],old=isOld();
  let feat=SMILEY.features_happy,fc=C.GREEN,rc;
  if(last){const l=level(last.sgv);
    if(l==='WL'||l==='UL'){feat=SMILEY.features_sad;fc=C.BLUE;}
    else if(l==='WH'||l==='UH'){feat=SMILEY.features_concerned;fc=C.RED;}
    if(old){feat=SMILEY.features_neutral;fc=C.GRAY;}
    rc=old?S.stale:levelColor(last.sgv);
  }else{feat=SMILEY.features_neutral;fc=S.stale;rc=S.stale;}
  bitmap(b,0,0,SMILEY.face_disc,8,8,fade(fc));
  bitmap(b,0,0,feat,8,8,C.BLACK);
  const s=last?printable(last.sgv):noDataText(),rw=24;
  let F=MU,ty=7,tw=textW(MU,s);
  if(tw>rw){F=AW;tw=textW(AW,s);ty=6;}
  drawText(b,F,s,8+Math.max(0,((rw-tw)/2)|0),ty,0,rc);
}},
{name:"Rainbow sparkle",draw(b,t){
  const last=READINGS[READINGS.length-1],old=isOld();
  let bmp=(((t/300)|0)%2===0)?SPARK.A:SPARK.B,fc=C.GREEN;
  if(last){const l=level(last.sgv),fall=(last.trend==='DOUBLE_DOWN'||last.trend==='SINGLE_DOWN');
    if(fall){bmp=SPARK.S;fc=C.YELLOW;}
    else if(l==='WH'||l==='UH'){bmp=SPARK.D;fc=C.MAGENTA;}
    else if(l==='WL'||l==='UL'){bmp=SPARK.S;fc=C.RED;}
  }
  if(!last||old)fc=S.stale;
  bitmap(b,0,0,bmp,8,8,fc);
  if(last){const s=printable(last.sgv),h0=((t/25)|0)&255;let x=9;
    for(let i=0;i<s.length;i++){drawText(b,AW,s[i],x,6,0,old?S.stale:hsv(h0+i*35));x+=textW(AW,s[i]);}
    trendArrow(b,last,27,1,old);
  }
}},
{name:"Time only",draw(b,t){
  const now=new Date();let h=now.getHours();const m=now.getMinutes(),s=now.getSeconds();let txt;
  if(S.h12){const ap=h<12?"AM":"PM";h=h%12===0?12:h%12;txt=`${h}:${String(m).padStart(2,'0')} ${ap}`;}
  else txt=`${String(h).padStart(2,'0')}:${String(m).padStart(2,'0')}:${String(s).padStart(2,'0')}`;
  drawText(b,AW,txt,0,6,2,C.WHITE);
}},
{name:"Rainbow clock",draw(b,t){
  const now=new Date();let h=now.getHours();const m=now.getMinutes(),s=now.getSeconds();let txt;
  if(S.h12){const ap=h<12?"AM":"PM";h=h%12===0?12:h%12;txt=`${h}:${String(m).padStart(2,'0')} ${ap}`;}
  else txt=`${String(h).padStart(2,'0')}:${String(m).padStart(2,'0')}:${String(s).padStart(2,'0')}`;
  const n=txt.length,h0=((t/20)|0)&255;let x=((32-textW(AW,txt))|0)/2;
  for(let i=0;i<n;i++){const hue=(((i*255)/Math.max(n,1))|0)+h0;drawText(b,AW,txt[i],x,6,0,hsv(hue));x+=textW(AW,txt[i]);}
}},
{name:"Nyan unicorn",draw(b,t,st){
  const last=READINGS[READINGS.length-1],old=isOld();
  const s=last?printable(last.sgv):noDataText(),vw=textW(AW,s);
  // Scroll state as a pure function of t (like Diagnostics): 0.37px per 33ms,
  // pause 3.5s with the unicorn parked right, then continue off and wrap.
  // (The old stepper froze: its catch-up loop re-triggered the pause every cycle.)
  const SPD=0.37/33,X0=-12,XP=24,XE=32+vw+18,T1=(XP-X0)/SPD,T2=T1+3500,T3=T2+(XE-XP)/SPD;
  const tt=t%T3;
  const ux=Math.trunc(tt<T1?X0+tt*SPD:(tt<T2?XP:XP+(tt-T2)*SPD));
  const leg=((t/110)|0)%2,wave=((t/110)|0)%4,tEnd=ux+1,tStart=ux-10;
  const bgX=tStart-vw-2,arrX=bgX+vw+1;
  const NY=[rgb565(255,0,55),rgb565(255,140,0),rgb565(255,235,0),rgb565(0,255,60),rgb565(160,40,255)];
  for(let x=Math.max(0,tStart);x<=Math.min(31,tEnd);x++){
    const seg=((((x/2)|0)+wave)%2),yB=seg===0?1:2;
    for(let q=0;q<5;q++)px(b,x,yB+q,NY[q]);
    if(((x+wave)%5)===0)px(b,x,seg===0?7:0,C.WHITE);
  }
  const GOLD=rgb565(255,215,0),PNK=rgb565(255,20,147),PNKL=rgb565(255,105,180),
        LBLU=rgb565(0,191,255),PPAL=rgb565(255,182,193);
  const P=(dx,dy,c)=>{const X=ux+dx;if(X>=0&&X<32&&dy>=0&&dy<8)px(b,X,dy,c);};
  P(6,0,GOLD);P(5,1,GOLD);P(4,1,PNK);P(3,2,PNKL);P(4,2,C.WHITE);P(6,2,C.WHITE);
  P(6,3,PPAL);P(5,2,LBLU);P(3,3,C.WHITE);P(4,3,C.WHITE);P(5,3,C.WHITE);
  for(let bx=2;bx<=5;bx++){P(bx,4,C.WHITE);P(bx,5,C.WHITE);}
  if(leg===0){P(2,6,C.WHITE);P(1,7,PNKL);P(5,6,C.WHITE);P(6,7,PNKL);}
  else{P(3,6,C.WHITE);P(2,7,PNKL);P(4,6,C.WHITE);P(5,7,PNKL);}
  const h0=((t/15)|0)&255;let cx=bgX;
  for(let i=0;i<s.length;i++){drawText(b,AW,s[i],cx,6,0,!last||old?S.stale:hsv(h0+i*35));cx+=textW(AW,s[i]);}
  if(last)trendArrow(b,last,arrX,1,old);
}},
{name:"Nyan cat",draw(b,t,st){
  const last=READINGS[READINGS.length-1],old=isOld();
  const s=last?printable(last.sgv):noDataText(),vw=textW(AW,s);
  // Same scroll as Nyan unicorn: 0.37px per 33ms, pause 3.5s parked right, then wrap.
  const SPD=0.37/33,X0=-12,XP=24,XE=32+vw+18,T1=(XP-X0)/SPD,T2=T1+3500,T3=T2+(XE-XP)/SPD;
  const tt=t%T3;
  const ux=Math.trunc(tt<T1?X0+tt*SPD:(tt<T2?XP:XP+(tt-T2)*SPD));
  const wave=((t/110)|0)%4,tEnd=ux+1,tStart=ux-10;
  const bgX=tStart-vw-2,arrX=bgX+vw+1;
  const NY=[rgb565(255,0,55),rgb565(255,140,0),rgb565(255,235,0),rgb565(0,255,60),rgb565(160,40,255)];
  for(let x=Math.max(0,tStart);x<=Math.min(31,tEnd);x++){
    const seg=((((x/2)|0)+wave)%2),yB=seg===0?1:2;
    for(let q=0;q<5;q++)px(b,x,yB+q,NY[q]);
    if(((x+wave)%5)===0)px(b,x,seg===0?7:0,C.WHITE);
  }
  // Nyan Cat: gray cat running right (head leads, body trails toward rainbow)
  const GRAY=rgb565(170,170,170),DKGRAY=rgb565(100,100,100),PNK=rgb565(255,150,200),
        PNKL=rgb565(255,105,180);
  const P=(dx,dy,c)=>{const X=ux+dx;if(X>=0&&X<32&&dy>=0&&dy<8)px(b,X,dy,c);};
  // Tail (up, trailing left)
  P(0,2,GRAY);P(0,3,GRAY);P(0,4,GRAY);
  // Cat body (gray)
  for(let bx=1;bx<=4;bx++){P(bx,2,GRAY);P(bx,3,GRAY);P(bx,4,GRAY);P(bx,5,GRAY);}
  // Cat head (gray with ears, leading right)
  P(5,0,GRAY);P(9,0,GRAY);  // ear tips
  P(5,1,GRAY);P(6,1,GRAY);P(8,1,GRAY);P(9,1,GRAY);  // ears + head top
  P(5,2,GRAY);P(6,2,DKGRAY);P(7,2,GRAY);P(8,2,DKGRAY);P(9,2,GRAY);  // eyes (dark)
  P(5,3,GRAY);P(6,3,PNK);P(7,3,GRAY);P(8,3,PNK);P(9,3,GRAY);  // cheeks (pink)
  P(6,4,GRAY);P(7,4,DKGRAY);P(8,4,GRAY);  // mouth
  // Legs (animated, like unicorn: gray with pink paws)
  const leg=((t/110)|0)%2;
  if(leg===0){P(1,6,GRAY);P(1,7,PNKL);P(3,6,GRAY);P(3,7,PNKL);}
  else{P(2,6,GRAY);P(2,7,PNKL);}
  const h0=((t/15)|0)&255;let cx=bgX;
  for(let i=0;i<s.length;i++){drawText(b,AW,s[i],cx,6,0,!last||old?S.stale:hsv(h0+i*35));cx+=textW(AW,s[i]);}
  if(last)trendArrow(b,last,arrX,1,old);
}},
{name:"Custom title scroll",draw(b,t,st){
  const last=READINGS[READINGS.length-1],old=isOld();
  const title="Nightscout";
  let bgS="---",dS="",towards=true;
  if(last){bgS=printable(last.sgv);
    if(READINGS.length>=2){const d=last.sgv-READINGS[READINGS.length-2].sgv;
      dS=(d>=0?"+":"")+printable(d);
      towards=(last.sgv>180&&d<0)||(last.sgv<70&&d>0)||(last.sgv>=70&&last.sgv<=180);}}
  const titleW=textW(AW,title),bgW=textW(AW,bgS),dW=dS?textW(AW,dS):0;
  const statsW=bgW+2+5+(dW>0?(3+dW):0),centerTarget=Math.max(0,Math.trunc((32-statsW)/2));
  // Scroll state as a pure function of t (like Diagnostics): 0.37px per 33ms,
  // pause 3.5s with the stats centered, then scroll off and wrap.
  // (The old stepper re-paused after every 1px past the pause point, crawling.)
  const SPD=0.37/33,XS=32,XQ=centerTarget-titleW-10,XE=-(titleW+10+statsW);
  const T1=(XS-XQ)/SPD,T2=T1+3500,T3=T2+(XQ-XE)/SPD,tt=t%T3;
  const qx=Math.trunc(tt<T1?XS-tt*SPD:(tt<T2?XQ:XQ-(tt-T2)*SPD));
  const curStats=qx+titleW+10,curBg=curStats,curArr=curBg+bgW+2,curDelta=curArr+5+3;
  drawText(b,AW,title,qx,6,0,C.CYAN);
  drawText(b,AW,bgS,curBg,6,0,old?S.stale:(last?levelColor(last.sgv):S.stale));
  if(last)trendArrow(b,last,curArr,1,old);
  if(dS)drawText(b,AW,dS,curDelta,6,0,old?S.stale:(towards?C.GREEN:C.YELLOW));
  if(last)timerBlocks(b,last,32,0,7);
}},
{name:"Unicorn",draw(b,t){
  const last=READINGS[READINGS.length-1],old=isOld();
  const l=last?level(last.sgv):'N';
  const pal=(!last||old)?[S.stale,S.stale,0x18C3,S.stale,S.stale,S.stale,S.stale,S.stale]
    :(l==='N'?UPALN:(l==='WL'||l==='WH')?[0xFE87,0xF79D,0x18C3,C.YELLOW,C.YELLOW,C.YELLOW,C.YELLOW,C.YELLOW]
    :[0xFE87,0xF79D,0x18C3,C.RED,C.RED,C.RED,C.RED,C.RED]);
  for(let yy=0;yy<8;yy++)for(let xx=0;xx<12;xx++){const idx=USPRITE[yy*12+xx];if(idx)px(b,xx,yy,pal[idx-1]);}
  drawText(b,AW,last?printable(last.sgv):noDataText(),31,6,1,!last||old?S.stale:levelColor(last.sgv));
  if(last)timerBlocks(b,last,16,16,7);
}},

{name:"Cat",draw(b,t){critterFace(b,"Cat");}},
{name:"Dog",draw(b,t){critterFace(b,"Dog");}},
{name:"Frog",draw(b,t){critterFace(b,"Frog");}},
{name:"Fox",draw(b,t){critterFace(b,"Fox");}},
{name:"Bunny",draw(b,t){critterFace(b,"Bunny");}},
{name:"Narwhal",draw(b,t){critterFace(b,"Narwhal");}},
{name:"Whale",draw(b,t){critterFace(b,"Whale");}},
{name:"Mario",draw(b,t){critterFace(b,"Mario");}},
{name:"Luigi",draw(b,t){critterFace(b,"Luigi");}},
{name:"Peach",draw(b,t){critterFace(b,"Peach");}},
{name:"Toad",draw(b,t){critterFace(b,"Toad");}},
{name:"Pumpkin",draw(b,t){critterFace(b,"Pumpkin");}},
{name:"Ghost",draw(b,t){critterFace(b,"Ghost");}},
{name:"Witch",draw(b,t){critterFace(b,"Witch");}},
{name:"Turkey",draw(b,t){critterFace(b,"Turkey");}},
{name:"Butterfly",draw(b,t){critterFace(b,"Butterfly");}},
{name:"Mario level",draw(b,t,st){
  const last=READINGS[READINGS.length-1],old=isOld();
  const s=last?printable(last.sgv):noDataText(),vw=textW(AW,s);
  const SPD=0.37/33;
  const LEVEL_LEN=70,POLE_WX=96;
  const T1=4500,T2=T1+1400,T3=T2+800,T4=T3+1000,T5=T4+1100,T6=T5+3500;
  const tt=t%T6, offF=T2*SPD, poleSX=POLE_WX-offF;
  const RED=rgb565(255,0,0),SKIN=rgb565(255,220,170),BLUE=rgb565(40,40,255),
        GRN=rgb565(0,200,0),GRNL=rgb565(140,255,140),YLW=rgb565(255,220,0),
        BRN=rgb565(150,90,30),BRND=rgb565(110,65,20),BLK=rgb565(0,0,0);
  const MR=[".RRR.","RRRRR","RSKSR",".SSS.","BBBBB",".BBB.",".B.B.",
            ".RRR.","RRRRR","RSKSR",".SSS.","BBBBB",".BBB.","..BB."];
  const MC={R:RED,S:SKIN,B:BLUE,K:BLK};
  const runF=((t/130)|0)%2;
  function drawMario(mx,my,frame){
    for(let r=0;r<7;r++)for(let c=0;c<5;c++){
      const ch=MR[frame*7+r][c];if(ch==='.')continue;
      const X=Math.round(mx)+c,Y=Math.round(my)+r;
      if(X>=0&&X<32&&Y>=0&&Y<8)px(b,X,Y,MC[ch]);
    }
  }
  let off=tt*SPD, mx=3, my=0, flagY=1, slide=false, showScene=true, bgX=32;
  if(tt>=T2&&tt<T3){
    off=offF;
    const pr=(tt-T2)/(T3-T2);
    mx=3+pr*((poleSX-2)-3);
    my=-Math.round(3.2*Math.sin(Math.PI*pr));
  }else if(tt>=T3&&tt<T4){
    off=offF;
    const pr=(tt-T3)/(T4-T3);
    mx=poleSX-2; my=Math.round(-3.2+3.2*pr); flagY=1+Math.round(3*pr); slide=true;
  }else if(tt>=T4&&tt<T5){
    const pr=(tt-T4)/(T5-T4);
    off=offF+pr*pr*70; mx=(POLE_WX-off)-2; my=0; flagY=4; slide=true;
    const statsW=vw+1+5,cxT=Math.max(0,Math.trunc((32-statsW)/2));
    bgX=Math.round(32-pr*(32-cxT));
  }else if(tt>=T5){
    showScene=false;
    const statsW=vw+1+5;bgX=Math.max(0,Math.trunc((32-statsW)/2));
  }
  if(showScene){
    for(let sx=0;sx<32;sx++){
      const wx=sx+off;
      if(wx<0||wx>POLE_WX+8)continue;
      if(wx<LEVEL_LEN)px(b,sx,7,(((wx|0)%4)<2)?BRN:BRND);
      const pm=((wx%36)+36)%36;
      if(wx>=28&&wx<LEVEL_LEN&&pm<2){
        for(let y=4;y<=6;y++)px(b,sx,y,GRN);
        px(b,sx,4,GRNL);
      }
      const cm=((wx%20)+20)%20;
      if(wx>=18&&wx<LEVEL_LEN&&cm<2){
        const spin=((t/240)|0)%2;
        if(!spin||cm===0){px(b,sx,2,YLW);px(b,sx,3,YLW);}
      }
      const bm=((wx%30)+30)%30;
      if(wx>=22&&wx<LEVEL_LEN&&bm<3){
        px(b,sx,4,BRN);px(b,sx,5,BRND);
      }
    }
    const fx=Math.round(POLE_WX-off);
    if(fx>=0&&fx<32){
      for(let y=0;y<=6;y++)px(b,fx,y,GRN);
      for(let dx=1;dx<=3;dx++)for(let dy=0;dy<2;dy++)if(fx+dx<32)px(b,fx+dx,flagY+dy,RED);
      if(fx+1<32){px(b,fx,7,GRNL);px(b,fx+1,7,GRNL);}
    }
    drawMario(mx,my,slide?1:runF);
  }
  if(bgX<32){
    let cx=bgX;
    for(let i=0;i<s.length;i++){drawText(b,AW,s[i],cx,6,0,!last||old?S.stale:levelColor(last.sgv));cx+=textW(AW,s[i]);}
    if(last)trendArrow(b,last,cx+1,1,old);
    if(last)timerBlocks(b,last,32,0,7);
  }
}},

];
"""

HTML_HEAD = """<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Nightscout Clock &mdash; Face Simulator</title>
<style>
  :root{color-scheme:dark}
  body{background:#0b0d10;color:#e8eaed;font-family:system-ui,-apple-system,sans-serif;margin:0;padding:16px}
  h1{font-size:20px;margin:0 0 4px}
  .sub{color:#9aa0a6;font-size:13px;margin-bottom:14px}
  .controls{display:flex;flex-wrap:wrap;gap:14px 22px;background:#15181d;border:1px solid #2a2f36;
    border-radius:10px;padding:12px 16px;margin-bottom:16px;align-items:center}
  .ctl{display:flex;align-items:center;gap:8px;font-size:13px}
  .ctl label{color:#9aa0a6}
  .ctl output{min-width:64px;font-variant-numeric:tabular-nums}
  input[type=range]{width:130px}
  select,button{background:#22262c;color:#e8eaed;border:1px solid #3a4048;border-radius:6px;padding:4px 8px;font-size:13px}
  .seg{display:flex;border:1px solid #3a4048;border-radius:6px;overflow:hidden}
  .seg button{border:0;border-radius:0;background:transparent;padding:4px 10px;cursor:pointer;color:#9aa0a6}
  .seg button.on{background:#2f6fed;color:#fff}
  .grid{display:grid;grid-template-columns:repeat(auto-fill,minmax(300px,1fr));gap:14px}
  .card{background:#15181d;border:1px solid #2a2f36;border-radius:10px;padding:10px 12px 12px}
  .card h2{font-size:13px;margin:0 0 8px;color:#c9ced4;font-weight:600}
  .card h2 .id{color:#5f6b76;font-weight:400;margin-right:6px}
  canvas{width:100%;image-rendering:pixelated;background:#000;border-radius:6px;display:block}
  .note{margin-top:14px;color:#5f6b76;font-size:12px}
</style>
</head>
<body>
<h1>Nightscout Clock &mdash; Face Simulator</h1>
<div class="sub">Pixel-exact preview of all {FACECOUNT} clock faces, rendered with the firmware&rsquo;s own font bitmaps and drawing logic. Animations run live.</div>
<div class="controls">
  <div class="ctl"><label>Glucose</label><input id="glucose" type="range" min="40" max="400" value="142"><output id="glucoseOut">142</output></div>
  <div class="ctl"><label>Trend</label><select id="trend">
    <option value="DOUBLE_UP">Double up</option><option value="SINGLE_UP">Single up</option>
    <option value="FORTYFIVE_UP">45&deg; up</option><option value="FLAT" selected>Flat</option>
    <option value="FORTYFIVE_DOWN">45&deg; down</option><option value="SINGLE_DOWN">Single down</option>
    <option value="DOUBLE_DOWN">Double down</option><option value="NONE">None</option>
  </select></div>
  <div class="ctl"><label>Data age</label><input id="age" type="range" min="0" max="40" value="3"><output id="ageOut">3 min</output></div>
  <div class="ctl"><label><input id="nodata" type="checkbox"> No data</label></div>
  <div class="ctl"><label>Units</label><div class="seg" id="units"><button data-v="0" class="on">mg/dL</button><button data-v="1">mmol/L</button></div></div>
  <div class="ctl"><label>Clock</label><div class="seg" id="clock"><button data-v="0" class="on">24h</button><button data-v="1">12h</button></div></div>
  <div class="ctl"><label>Stale color</label><input id="stale" type="color" value="#a451a4"></div>
</div>
<div class="grid" id="grid"></div>
<div class="note">Stale threshold: 20 min (matches firmware default). Timer blocks: one per minute of data age, up to 5. Sample history is synthesized: 3 h of 5-minute readings ending at the chosen value and trend.</div>
<script>
"""

HTML_TAIL = r"""
// ================= ui =================
const grid=document.getElementById('grid');
const states=FACES.map(()=>({}));
const canvases=FACES.map((f,i)=>{
  const card=document.createElement('div');card.className='card';
  const h=document.createElement('h2');h.innerHTML=`<span class="id">${i}</span>${f.name}`;
  const cv=document.createElement('canvas');cv.width=32*15;cv.height=8*15;
  card.appendChild(h);card.appendChild(cv);grid.appendChild(card);
  return cv.getContext('2d');
});
const PX=15;
function renderFace(ctx,b){
  ctx.fillStyle='#000';ctx.fillRect(0,0,32*PX,8*PX);
  for(let y=0;y<8;y++)for(let x=0;x<32;x++){
    const c=b[y*32+x];if(!c)continue;
    ctx.fillStyle=css(c);
    ctx.fillRect(x*PX+1,y*PX+1,PX-2,PX-2);
  }
}
const t0=performance.now();
function frame(){
  const t=performance.now()-t0;
  for(let i=0;i<FACES.length;i++){
    const b=new Uint16Array(32*8);
    try{FACES[i].draw(b,t,states[i]);}catch(e){}
    renderFace(canvases[i],b);
  }
  requestAnimationFrame(frame);
}
function hexTo565(h){const r=parseInt(h.slice(1,3),16),g=parseInt(h.slice(3,5),16),bl=parseInt(h.slice(5,7),16);
  return rgb565(r,g,bl);}
const $=id=>document.getElementById(id);
function refresh(){
  S.value=+$('glucose').value;S.trend=$('trend').value;
  S.ageMin=+$('age').value;S.noData=$('nodata').checked;
  S.stale=hexTo565($('stale').value);
  $('glucoseOut').textContent=S.value+(S.mmol?" ("+(S.value/18).toFixed(1)+")":"");
  $('ageOut').textContent=S.ageMin+" min"+(S.ageMin>=20?" (stale)":"");
  buildReadings();
}
document.querySelectorAll('#units button').forEach(btn=>btn.onclick=()=>{
  document.querySelectorAll('#units button').forEach(x=>x.classList.remove('on'));
  btn.classList.add('on');S.mmol=btn.dataset.v==='1';refresh();});
document.querySelectorAll('#clock button').forEach(btn=>btn.onclick=()=>{
  document.querySelectorAll('#clock button').forEach(x=>x.classList.remove('on'));
  btn.classList.add('on');S.h12=btn.dataset.v==='1';refresh();});
['glucose','trend','age','nodata','stale'].forEach(id=>$(id).addEventListener('input',refresh));
refresh();
requestAnimationFrame(frame);
</script>
</body>
</html>
"""

face_count = JS_FACES.count('{name:')
html = HTML_HEAD.replace('{FACECOUNT}', str(face_count)) + JS_DATA + JS_ENGINE + JS_FACES + HTML_TAIL
open(os.path.join(HERE, 'index.html'), 'w').write(html)
print("faces:", face_count, "bytes:", len(html))
