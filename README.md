# Projeto com ESP32

Este projeto consiste em configurar uma rede de comunicação entre quatro dispositivos ESP32, onde um deles irá enviar dados e os outros três irão receber. Além disso, um dos dispositivos receptores terá uma função de redundância (OU EXCLUSIVO).

## Funcionalidades

- Um ESP32 será configurado como emissor de dados.
- Três ESP32 serão configurados como receptores de dados.
- Um dos receptores terá uma função de OU EXCLUSIVO.

## Componentes Necessários

- 1 ESP32 para o emissor de dados.
- 3 ESP32 para os receptores de dados.
- Comunicação entre os dispositivos.

## Configuração do Projeto

1. **Código Fonte:**
   - O código fonte está disponível nos arquivos `ESPNOW_Basic_Master.ino`, `basic_slave.ino` e `receptor_unico.ino`.
   - O arquivo `ESPNOW_Basic_Master.ino` contém o código para o ESP32 emissor de dados.
   - O arquivo `basic_slave.ino` contém o código para os ESP32 receptores de dados.
   - O arquivo `receptor_unico.ino` contém o código para o ESP32 que terá a função adicional exclusiva.

2. **Bibliotecas Necessárias:**
   - Este projeto utiliza as bibliotecas `esp_now.h` e `WiFi.h` para configuração da comunicação ESP-NOW e da conexão Wi-Fi.

3. **Configuração do ESP-NOW:**
   - Todos os dispositivos ESP32 devem ser configurados para usar o ESP-NOW para comunicação entre si.
   
## Execução do Projeto

1. **Inicialização:**
   - Carregue o código fonte nos quatro dispositivos ESP32 usando a IDE Arduino ou outra ferramenta de programação compatível.
   - Inicie os dispositivos e observe as mensagens de inicialização para garantir que a comunicação esteja estabelecida corretamente.

2. **Operação:**
   - O dispositivo configurado como emissor enviará os dados periodicamente.
   - Os dispositivos configurados como receptores receberão os dados do emissor.
   - Um dos dispositivos receptores, configurado com a função adicional exclusiva, terá uma operação específica que será ativada em determinadas condições.

## Contribuição

Sinta-se à vontade para sugerir melhorias, correções de bugs ou novos recursos através de pull requests.
