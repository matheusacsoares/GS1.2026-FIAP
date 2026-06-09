# GS 1.2026 - Sistema de Monitoramento para Nanossatélite

## Integrantes

* Giovanna Ferreira Almeida | RM: 571822
* Maria Luiza Vieira Freitas | RM: 571535
* Matheus Arruda Camara Soares | RM: 571594


---

# Sumário

1. [Sobre o Projeto](#sobre-o-projeto)
2. [Objetivos da Solução](#objetivos-da-solução)
3. [Tecnologias Utilizadas](#tecnologias-utilizadas)
4. [Componentes Utilizados](#componentes-utilizados)
5. [Arquitetura Geral do Sistema](#arquitetura-geral-do-sistema)
6. [Entrega - Computer Organization and Architecture (COA)](#entrega---computer-organization-and-architecture-coa)
7. [Entrega - Soluções em Energias Renováveis e Sustentabilidade (SERS)](#entrega---soluções-em-energias-renováveis-e-sustentabilidade-sers)
8. [Estrutura das Telas](#estrutura-das-telas)
9. [Como Executar o Projeto](#como-executar-o-projeto)
10. [Estrutura do Código](#estrutura-do-código)
11. [Possíveis Evoluções da Solução](#possíveis-evoluções-da-solução)

---

# Sobre o Projeto

Este projeto foi desenvolvido para a Global Solution (GS) do primeiro semestre de 2026.

A proposta consiste em um protótipo de sistema embarcado para monitoramento de um nanossatélite. O sistema é responsável por coletar informações de sensores, processar os dados localmente, gerar alertas operacionais e realizar o gerenciamento energético da missão.

O projeto foi desenvolvido utilizando um ESP32 no ambiente de simulação Wokwi e integra conteúdos estudados nas disciplinas de:

* Computer Organization and Architecture (COA)
* Soluções em Energias Renováveis e Sustentabilidade (SERS)

---

# Objetivos da Solução

O sistema tem como objetivo:

* Monitorar condições operacionais do nanossatélite.
* Detectar situações de alerta relacionadas à temperatura e vibração.
* Simular a geração de energia por painéis solares.
* Simular o armazenamento de energia em baterias.
* Estimar a autonomia energética da missão.
* Demonstrar conceitos de processamento de dados embarcados.
* Integrar conteúdos de diferentes disciplinas em uma única solução.

---

# Tecnologias Utilizadas

* Microcontrolador ESP32
* Linguagem C++
* Arduino Framework
* Wokwi Simulator


---

# Componentes Utilizados

| Componente                   | Função no Projeto                                         |
| ---------------------------- | --------------------------------------------------------- |
| ESP32-S2 DevKit              | Computador de bordo do nanossatélite                      |
| DHT22                        | Monitoramento de temperatura                              |
| MPU6050                      | Monitoramento de aceleração, vibração e impactos          |
| LDR (Photoresistor)          | Simulação da incidência solar sobre o painel fotovoltaico |
| LCD 20x4 I2C                 | Exibição das informações do sistema                       |
| LED Bar Graph (10 segmentos) | Indicador visual do nível da bateria                      |
| LED Vermelho                 | Alerta de temperatura                                     |
| LED Vermelho                 | Alerta de bateria crítica                                 |
| LED Laranja                  | Alerta de impacto/vibração                                |
| LED Laranja                  | Alerta de déficit energético                              |
| Resistores 220 Ω             | Proteção dos LEDs                                         |
| Protoboard                   | Montagem do circuito virtual                              |


---

# Arquitetura Geral do Sistema

O sistema opera em quatro etapas principais:

### 1. Aquisição de Dados

Os sensores realizam a coleta das informações do ambiente.

* Temperatura
* Vibração
* Luminosidade

### 2. Processamento

O ESP32 interpreta os dados recebidos e executa os cálculos necessários para determinar o estado atual da missão.

### 3. Tomada de Decisão

Com base nos valores obtidos, o sistema identifica situações de:

* Operação normal
* Atenção
* Alerta
* Condições críticas

### 4. Exibição

Os resultados são apresentados através do display LCD e dos LEDs de sinalização.

---

# Entrega - Computer Organization and Architecture (COA)

Esta parte do projeto foi desenvolvida para demonstrar conceitos relacionados a sistemas embarcados, processamento de informações e integração entre hardware e software.

## Monitoramento de Temperatura

Utilizando o sensor DHT22, o sistema realiza:

* Leitura da temperatura atual.
* Cálculo da variação térmica.
* Identificação da tendência da temperatura.
* Classificação do estado operacional.

### Estados de Temperatura

* Operacional
* Atenção
* Alerta
* Crítico

---

## Monitoramento de Vibração

O sensor MPU6050 é utilizado para monitorar acelerações e vibrações do sistema.

Os dados dos três eixos são processados para gerar uma aceleração resultante.

### Estados de Vibração

* Normal
* Anormal
* Turbulência
* Impacto

---

## Processamento e Tomada de Decisão

O software interpreta continuamente os dados recebidos pelos sensores.

A partir desses dados são gerados:

* Alertas automáticos.
* Classificações operacionais.
* Indicadores de estado.

Essa etapa representa o processamento realizado pelo computador de bordo do nanossatélite.

---

## Sistema de Alertas

O sistema utiliza LEDs para indicar condições críticas identificadas durante o processamento.

Alertas monitorados:

* Temperatura crítica.
* Vibração anormal.
* Bateria crítica.
* Déficit energético.

---

## Interface Homem-Máquina

O display LCD apresenta as informações processadas pelo sistema de forma organizada e acessível.

---

# Entrega - Soluções em Energias Renováveis e Sustentabilidade (SERS)

Esta parte do projeto foi desenvolvida para representar o subsistema energético do nanossatélite.

---

## Simulação de Painel Solar

Como o ambiente de simulação não possui um painel fotovoltaico real, foi utilizado um LDR para representar a incidência solar.

Com base na luminosidade recebida, o sistema estima a potência gerada pelo painel.

### Estados Simulados

* Sol Pleno
* Transição Orbital
* Eclipse Orbital

---

## Potência Gerada

O sistema calcula uma estimativa da potência produzida pelo painel fotovoltaico simulado.

Unidade utilizada:

* Watt (W)

---

## Potência Consumida

Também é realizada uma estimativa do consumo energético do módulo.

O cálculo considera:

* Consumo base do sistema.
* Consumo dos sensores.
* Consumo adicional causado por alertas ativos.

Unidade utilizada:

* Watt (W)

---

## Saldo Energético

O saldo energético é calculado pela diferença entre:

Potência Gerada - Potência Consumida

Esse valor permite identificar se o sistema está:

* Carregando a bateria.
* Descarregando a bateria.
* Operando em equilíbrio energético.

---

## Simulação de Bateria

O armazenamento de energia é representado através de uma bateria virtual.

A energia armazenada é medida em:

* Watt-hora (Wh)

Informações monitoradas:

* Energia armazenada.
* Capacidade máxima.
* Percentual de carga.

---

## Autonomia Energética

O sistema estima quanto tempo o nanossatélite pode continuar operando utilizando a energia disponível na bateria.

Essa informação é calculada utilizando:

* Energia disponível.
* Consumo atual.

Resultado apresentado em:

* Horas

---

## Indicador Visual de Carga

Uma barra de LEDs representa visualmente o nível atual da bateria.

Quanto maior a carga disponível, maior a quantidade de LEDs acesos.

---

# Estrutura das Telas

O display LCD foi organizado em múltiplas páginas para facilitar o monitoramento.

## Tela 1 - Alertas

Exibe todos os alertas ativos do sistema.

---

## Tela 2 - Temperatura

Exibe:

* Temperatura atual.
* Variação térmica.
* Tendência.

---

## Tela 3 - Painel Solar

Exibe:

* Luminosidade.
* Potência gerada.
* Estado orbital.

---

## Tela 4 - Vibração

Exibe:

* Aceleração total.
* Estado de vibração.

---

## Tela 5 - Energia

Exibe:

* Potência gerada.
* Potência consumida.
* Saldo energético.

---

## Tela 6 - Bateria

Exibe:

* Percentual de carga.
* Energia armazenada.
* Estado da bateria.

---

## Tela 7 - Autonomia

Exibe a autonomia estimada da missão.

---

# Como Executar o Projeto

1. Abrir o projeto no Wokwi.
2. Iniciar a simulação.
3. Alterar os sensores para observar diferentes comportamentos do sistema.
4. Acompanhar as informações através do LCD e dos LEDs.

---

# Estrutura do Código

O código foi organizado em funções independentes para facilitar a manutenção e a identificação dos requisitos de cada disciplina.

Principais módulos:

* `temperatura()`
* `mpuLoop()`
* `painelSolar()`
* `calcularConsumo()`
* `atualizarEnergia()`
* `alertasBoard()`
* `gerenciarPaginas()`
* `atualizarDisplay()`

Cada função possui comentários explicativos indicando sua relação com as disciplinas COA e SERS.

---

# Possíveis Evoluções da Solução

Como continuação do projeto, podem ser implementadas funcionalidades adicionais, como:

* Sistema de telemetria para envio dos dados para uma estação terrestre.
* Armazenamento histórico dos dados coletados.
* Dashboard externo para monitoramento remoto.
* Simulação mais avançada de geração fotovoltaica.
* Controle automático de economia de energia.

---

# Considerações Finais

O projeto permitiu integrar conceitos de sistemas embarcados, processamento de dados e gestão energética em uma única solução.

A proposta demonstra como um nanossatélite pode monitorar seu estado operacional, identificar situações críticas e administrar seus recursos energéticos de forma autônoma, utilizando os conhecimentos desenvolvidos ao longo do semestre.
