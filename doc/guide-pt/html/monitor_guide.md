
# Montagem do Experimento

Esta parte do guia visa mostrar o passo a passo para a montagem do experimento com Neutrons utilizando o PSoC 6.

## Pré-requisitos

- Certifique-se que o sistema operacional não será suspenso ou desligado automaticamente nas opções de energia do computador; 
- Obtenha o software "monitor_psoc" para o sistema operacional escolhido;
- Tenha em mãos os dois kits de desenvolvimento e os cabos USB para a conexão dos dispositivos com a interface serial do software. 

**Figure 1. PSoC 6 BLE Kit - Design Under Test(DUT)**

\begin{figure}[ht]
    \centering
    \includegraphics[width=250px]{imgs/psoc63_DUT.png}
    \caption{Alt text}
\end{figure}


**Figure 2. PSoC 6 WI-fi Kit - External Watchdog**

![drawing](imgs/psoc62_watchdog.png)


## Etapas

1. Realize a conexão das placas conforme está representado no diagrama da figura abaixo:


2. Abra o software de monitoramento, entre em "Start Monitoring" e escolha porta serial que cada dispositivo está conectado.

Para descobrir a porta serial dos dipositivos, o usuário poderá entrar em "Gerenciador de Dispositivos" no windows coforme a figura abaixo:

Ou então, se o sistema operacional for linux, listar as portas utlizando: "ls /dev/ttyUSB* /dev/ttyACM*". Caso o usuário tente conectar o kit errado, o software acusará "Error connecting to device" e então basta selecionar a outra porta serial.

Exemplo windows:


Exemplo linux:


3. Adicione informações sobre o experimento em "Additional log info" como o fluxo de neutrons escolhido ou a temperatura do local. Essa informação será adicionada ao cabeçalho do arquivo de log da sessão iniciada.

5. Registre uma foto da montagem do exemperimento para ser encaminhado juntamente com os arquivos .log. 

# Atualizando o Firmware

Caso seja necessário regravar a imagem da flash do dispositivo



