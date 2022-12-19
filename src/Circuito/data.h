/* Este módulo coordena a inicialização dos parâmetros mínimos e máximos de umidade e temperatura.
Se houver conexão com a internet, isso é feito através de uma requisição ao servidor.
Se não, consulta-se os dados salvos na memória do ESP. */

// Inclui bibliotecas necessárias
#include <Preferences.h> // Biblioteca que permite o acesso à memória do ESP para guardar pares de chave-valor
#include "requestParameters.h" // Módulo personalizado para se comunicar com o servidor e fazer requisição de parâmetros

// Inicia variáveis a serem utilizadas no código principal
double tempMin, tempMax, humidityMin, humidityMax;
// Inicia array para guardar parâmetros
double parameters[] = { 0, 0, 0, 0 };

void initCurrentParameters() {
  Preferences preferences; // Cria objeto de preferências (para salvar dados persistentemente)
  preferences.begin("greener", false); // Abra namespace (espaço de memória)
  if (getWiFiStatus()) { // Se tiver conectado à internet...
    requestParameters(parameters); // Atualiza array com valores do servidor

    // Desmembra array em variáveis
    tempMin = parameters[0];
    tempMax = parameters[1];
    humidityMin = parameters[2];
    humidityMax = parameters[3];

    // Salva valores nas preferências
    preferences.putInt("tempMin", tempMin);
    preferences.putInt("tempMax", tempMax);
    preferences.putInt("humidityMin", humidityMin);
    preferences.putInt("humidityMax", humidityMax);
  } else { // Se não tiver internet, pega últimos valores salvos
    tempMin = preferences.getInt("tempMin");
    tempMax = preferences.getInt("tempMax");
    humidityMin = preferences.getInt("humidityMin");
    humidityMax = preferences.getInt("humidityMax");
  }
}