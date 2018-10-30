<CsoundSynthesizer>
<CsOptions>
-odac -d
</CsOptions>
<CsInstruments>

sr = 44100 ; частота дискретизации
ksmps = 32 ; сигналы автоматики, это низкочастотные сигналы
nchnls = 2 ; количество каналов для выхода
0dbfs = 1  ; какое относительное значение амплитуды мы принимаем за 0 дб

gaL init 0 ; инициализация глобальной переменной
gaR init 0

instr 1
	; a - тип данных с высокой частотой дискретизации
	a1 rand 0.5 ;a1 - один выход, 0.5 - входные параметры
	outs a1, a1 
endin


instr 2
	kEnv linseg 0, .8, 1, 4, 0.2 ; амплитудная модуляция
	a1 oscil .7, 200
	a2 oscil 0.4, 300
	a3 oscil .12, 70
	a4 oscil .5, 250

	aMix = kEnv*(a1+a2+a3+a4)/4
	outs aMix, aMix
endin

; субтрактивный сигнал

instr 3
	i1 = cpspch(p4)
	a1 vco2 0.5, i1
	a2 vco2 0.5, i1*1.001
	a3 vco2 0.5, i1*0.998
	aM = (a1+a2+a3)
	aF	 butterlp aM, 500
	outs aF,aF
endin

instr 4
 aN rand 1
 
 a1 butterbp aN, 400, 20
 a2 butterbp aN, 213, 20
 a3 butterbp aN, 333, 20
 a4 butterbp aN, 552, 20
 
 aL = (a1+a2+a3*.5+a4*.5)
 aR = (.4*a1+.4*a2+a3+a4)
 
 outs aL, aR
 gaL += aL * .4
 gaR += aR * .4
endin

instr 100

	a1, a2 reverbsc gaL, gaR, .9, 10000
	outs a1, a2
	gaL = 0
	gaR = 0

endin

</CsInstruments>
<CsScore>
i4 0 10
i100 0 10
</CsScore>
</CsoundSynthesizer>
<bsbPanel>
 <label>Widgets</label>
 <objectName/>
 <x>100</x>
 <y>100</y>
 <width>320</width>
 <height>240</height>
 <visible>true</visible>
 <uuid/>
 <bgcolor mode="nobackground">
  <r>255</r>
  <g>255</g>
  <b>255</b>
 </bgcolor>
</bsbPanel>
<bsbPresets>
</bsbPresets>
