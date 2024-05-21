# Projeto de Automação FOG Computing com ESP32 

O projeto visa implementar um sistema de automação utilizando a tecnologia de FOG computing, aproveitando os recursos dos dispositivos ESP32 para processamento local dos dados. Cada ESP32 desempenha um papel específico na automação:

- 1 dispositivo "master" ESP32 que envia comandos e dados para os outros dispositivos.
- 2 dispositivos "slave" ESP32 (A e B) que executam tarefas específicas com base nos comandos recebidos do dispositivo "master".
- 1 dispositivo de paridade ESP32 que calcula a paridade dos dados recebidos dos dispositivos "slave" A e B.
- 1 dispositivo "cloud" ESP32 que verifica a paridade dos dados antes de enviá-los para a nuvem, garantindo sua integridade.

## Próximos Passos

1. **Configuração dos Dispositivos ESP32:**
   - Certifique-se de configurar cada dispositivo ESP32 com o código fornecido neste repositório, conforme as instruções específicas para cada dispositivo.

2. **Testes de Comunicação e Processamento:**
   - Realize testes para garantir que os dispositivos ESP32 estejam se comunicando corretamente e que o processamento dos dados esteja ocorrendo conforme o esperado.

3. **Implementação da Lógica de Paridade:**
   - Desenvolva a lógica necessária para calcular a paridade dos conjuntos de dados recebidos dos dispositivos "slave" A e B.

4. **Integração com a Nuvem:**
   - Configure o dispositivo "cloud" para verificar a paridade dos dados e enviar para a nuvem somente se estiverem corretos.

5. **Testes Finais e Depuração:**
   - Realize testes finais para garantir que o sistema esteja funcionando corretamente e depure quaisquer problemas encontrados.

## Contribuição

Sinta-se à vontade para sugerir melhorias, correções de bugs ou novos recursos através de pull requests.
