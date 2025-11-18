# Guía Completa para PCB en KiCAD 9 - Amplificador TPA3116D2

## 📋 Configuración del Proyecto

### 1. Configuración Inicial de KiCAD 9
```
File → New Project
Nombre sugerido: amp_clase_d_tpa3116
```

### 2. Configuración de Capas (Board Setup)
```
Capas recomendadas (PCB 2 capas):
- Top Layer (F.Cu): Componentes + señales
- Bottom Layer (B.Cu): Ground plane principalmente
- F.SilkS / B.SilkS: Serigrafía
- F.Mask / B.Mask: Máscara de soldadura
```

### 3. Reglas de Diseño (Design Rules)
```
Board Setup → Constraints:

Clearances:
- Mínimo: 0.25mm (10mil)
- Preferido: 0.5mm (20mil) para señales de potencia

Track Widths:
- Señales de audio: 0.3mm (12mil)
- Señales de control: 0.25mm (10mil)
- AVDD: 0.8mm (32mil)
- PVDD: 2.5-4mm (100-160mil) ⚠️ CRÍTICO
- OUT± (salidas): 2.5-4mm (100-160mil) ⚠️ CRÍTICO

Vias:
- Señales normales: 0.8mm pad, 0.4mm drill
- Power: 1.2mm pad, 0.6mm drill
```

## 🔌 Layout de Componentes - Reglas Críticas

### Zona del IC (TPA3116D2)
```
PRIORIDAD MÁXIMA:

1. Colocar TPA3116D2 en el CENTRO del PCB
2. Orientación: Pines de salida hacia los conectores de parlantes
3. Espacio libre alrededor: mínimo 10mm en todos lados
4. Pad térmico inferior debe conectarse a PGND con múltiples vías térmicas
```

### Filtros LC de Salida (CRÍTICO ⚡)
```
Ubicación: Lo MÁS CERCA POSIBLE de pines OUT±

Distancia máxima pin→inductor: 3mm
Distancia inductor→capacitor: 3mm

Configuración:
┌─────────────────────────────────┐
│ TPA3116D2                       │
│         OUT_L+ ──→ L1 ──→ C6    │ ← 5mm máximo
│         OUT_L- ──→ L2 ──→ C7    │
└─────────────────────────────────┘

NO uses vías entre el pin y el primer inductor
Usa pistas anchas y cortas (3-4mm ancho)
```

### Condensadores de Desacoplamiento
```
Ubicación respecto a pines del IC:

C2 (100µF): ≤ 5mm de PVDD
C3 (0.1µF): ≤ 2mm de AVDD (lo más cerca posible)
C10, C11 (Bootstrap): ≤ 3mm de pines BSL/BSR

Técnica: Coloca los cerámicos más cerca que los electrolíticos
```

### Área de Entrada (Audio Input)
```
Mantener SEPARADA del área de potencia (mínimo 20mm)

Orden de colocación:
[Conector] → [C4/C5] → [RV1/RV2] → [R2/R3] → [IC pines IN]

R2/R3 (pull-down) cerca de los pines IN del IC
Pistas de señal: alejadas de PVDD y pistas OUT±
```

## 🌐 Ground Planes - Sistema Star Ground

### Configuración de Planos de Tierra
```
⚠️ SUPER IMPORTANTE: Separar grounds analógico y digital

Bottom Layer:
┌──────────────────────────────────────┐
│  AGND Zone    │    PGND Zone         │
│  (Audio)      │    (Power)           │
│               │                      │
│   Star Point ──── Single Connection  │
│    (cerca del IC)                    │
└──────────────────────────────────────┘

Star Point: Ubicarlo debajo del TPA3116D2
Conexión: Una pista ancha (3mm) o múltiples vías
```

### KiCAD 9 - Creación de Zonas:
```
1. Selecciona "Add Filled Zone" (B.Cu layer)
2. Crea zona PGND: conecta a net "PGND"
   - Priority: 1
   - Clearance: 0.5mm
3. Crea zona AGND: conecta a net "AGND"  
   - Priority: 1
   - Clearance: 0.5mm
4. Mantén 5mm de separación entre zonas
5. Únelas solo en el star point con pista de 3mm
```

## 📏 Dimensiones del PCB

### Tamaño Recomendado
```
PCB mínimo: 80mm x 60mm
PCB recomendado: 100mm x 70mm
PCB con disipador: 110mm x 80mm

Grosor: 1.6mm (estándar)
Cobre: 35µm (1oz) mínimo, 70µm (2oz) preferido
```

### Agujeros de Montaje
```
4 agujeros de montaje M3 (3.2mm drill)
Ubicación: 3-5mm de cada esquina
Agregar zona sin cobre (keepout) de 5mm alrededor
```

## 🔥 Gestión Térmica

### Pad Térmico del IC
```
TPA3116D2 tiene pad térmico inferior (DAP)

En KiCAD:
1. Asegúrate que el footprint tenga el pad térmico
2. Conecta DAP a PGND
3. Usa matriz de vías térmicas:

     Via  Via  Via
     Via  Via  Via    ← 8-12 vías
     Via  Via  Via       0.6mm drill
     
Espaciado entre vías: 1.5-2mm
```

### Disipador (Opcional)
```
Si Pout > 30W por canal:
- Disipador de aluminio TO-220 o mayor
- Pasta térmica
- Tornillo M3
- Aislante si es necesario
```

## 🛡️ Consideraciones EMI/EMC

### Pistas de Alta Frecuencia (OUT±)
```
Características de las salidas PWM:
- Frecuencia: ~400kHz
- Alta corriente: hasta 5A
- Genera mucho ruido EMI

Contramedidas:
1. Pistas lo más cortas posible
2. Pistas OUT+ y OUT- en paralelo (acoplamiento diferencial)
3. Ground plane continuo debajo
4. Ferrite bead (FB1) en línea PVDD
5. Filtro LC bien diseñado
```

### Separación de Señales
```
Mantener SEPARADAS:

[Audio Input] ←─── 20mm mínimo ───→ [Power/Output]
[AGND]        ←─── Star Point  ───→ [PGND]

Evitar cruce de pistas:
- Pistas IN no deben cruzar debajo de OUT
- AVDD separado de PVDD
```

## 🔧 Footprints en KiCAD 9

### Librerías Necesarias
```
IC Principal:
TPA3116D2: HTSSOP-32 (Thermal pad)
Librería: Texas_Instruments o crear custom

Si no existe:
1. Symbol: crear desde datasheet (pág. 2-3)
2. Footprint: HTSSOP-32 con thermal pad
   - Pitch: 0.65mm
   - Thermal pad: ~4x4mm central
```

### Footprints Personalizados
```
Inductores (L1-L4):
- Footprint: según fabricante (ej: Bourns SRR1260)
- Tamaño típico: 12x12mm
- Pad spacing: según datasheet

Conectores de parlantes:
- Borneras 5mm pitch
- 2 posiciones por canal
```

## ⚡ Checklist Pre-Fabricación

### Revisión de Esquemático
- [ ] Todas las conexiones verificadas
- [ ] Valores de componentes correctos
- [ ] Reglas ERC pasadas (0 errores)
- [ ] Números de part numbers agregados

### Revisión de PCB
- [ ] DRC pasado (0 errores críticos)
- [ ] Ground planes sin islas flotantes
- [ ] Star ground implementado correctamente
- [ ] Pistas de potencia ≥ 2.5mm
- [ ] Filtros LC a ≤ 5mm del IC
- [ ] Vías térmicas en pad térmico
- [ ] Clearance 0.5mm para PVDD/OUT
- [ ] Serigrafía: valores y referencias visibles
- [ ] Agujeros de montaje presentes

### Archivos de Fabricación (Gerbers)
```
File → Fabrication Outputs → Gerbers (.gbr)

Capas necesarias:
- F.Cu (Top copper)
- B.Cu (Bottom copper)
- F.SilkS / B.SilkS (Silkscreen)
- F.Mask / B.Mask (Solder mask)
- Edge.Cuts (Board outline)
- F.Paste / B.Paste (Si usas stencil)

Drill files (.drl):
- PTH (Plated through-hole)
- NPTH (Non-plated, si aplica)

Comprimir todo en ZIP para fabricante
```

## 📦 BOM (Bill of Materials)

### Generación en KiCAD 9
```
Tools → Generate BOM

Información incluir:
- Reference (C1, R1, etc)
- Value (100µF, 10kΩ, etc)
- Footprint
- Quantity
- Part Number (MPN)
- Supplier (Mouser, Digikey, etc)
- Notas especiales (Low ESR, Film, etc)
```

### Proveedores Recomendados
```
México:
- Electrónica Steren
- SASA Electrónica
- Compel Electrónica

Internacional (envío a MX):
- Mouser Electronics
- Digikey
- LCSC (económico)
- Arrow Electronics
```

## 🎯 Tips Específicos para Principiantes

### Errores Comunes a Evitar
1. ❌ Pistas de potencia muy delgadas → ✅ Mínimo 2.5mm
2. ❌ Filtro LC lejos del IC → ✅ Máximo 5mm
3. ❌ Grounds mezclados → ✅ Star ground
4. ❌ Sin pad térmico conectado → ✅ Vías térmicas a PGND
5. ❌ Condensadores de desacoplamiento lejos → ✅ Lo más cerca posible

### Simulación antes de Fabricar
```
Usar simulador SPICE en KiCAD 9:
1. Verifica voltajes en nodos clave
2. Revisa corrientes en pistas de potencia
3. Confirma filtro LC (frecuencia de corte ~30kHz)
```

### Primera Vez - Recomendaciones
```
1. Ordena PCB de prueba en servicio económico (JLCPCB, PCBWay)
2. No ordenes más de 5 piezas la primera vez
3. Revisa 3 veces antes de ordenar
4. Ten multímetro listo para pruebas
5. Conecta primero sin IC para verificar voltajes
```

## 📞 Recursos Adicionales

### Documentación Oficial
- TPA3116D2 Datasheet: [ti.com](https://www.ti.com/product/TPA3116D2)
- KiCAD 9 Manual: [docs.kicad.org](https://docs.kicad.org)
- Application Note: SLOA119 - Clase D Amplifiers

### Comunidades
- Foro KiCAD: forum.kicad.info
- r/PrintedCircuitBoard
- r/AskElectronics
- EEVblog Forum

---

**¿Necesitas ayuda específica con algún paso?** ¡Pregúntame sobre:
- Creación de footprints custom
- Configuración de reglas DRC
- Troubleshooting de errores comunes
- Selección de componentes específicos