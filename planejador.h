#ifndef _PLANEJADOR_H_
#define _PLANEJADOR_H_

#include <string>
#include <list>
#include <cmath>
#include <iostream>

const double PI = 3.14159265358979323846;
const double EARTH_RADIUS_KM = 6371.0;

/* *************************
   * CLASSE IDPONTO        *
   ************************* */

/// Identificador de um Ponto
class IDPonto
{
private:
    std::string t;
public:
    // Construtor
    IDPonto() : t("") {}
    
    // Atribuição de string temporária
    void set(std::string&& S) { t = std::move(S); }

    // Teste de validade
    bool valid() const { return (t.size() >= 2 && t[0] == '#'); }

    // Comparação
    bool operator==(const IDPonto& ID) const { return t == ID.t; }
    bool operator!=(const IDPonto& ID) const { return !operator==(ID); }

    // Impressão
    friend std::ostream& operator<<(std::ostream& X, const IDPonto& ID) {
        return X << ID.t;
    }
};

/* *************************
   * CLASSE IDROTA         *
   ************************* */

/// Identificador de uma Rota
class IDRota
{
private:
    std::string t;
public:
    // Construtor
    IDRota() : t("") {}

    // Atribuição de string temporária
    void set(std::string&& S) { t = std::move(S); }

    // Teste de validade
    bool valid() const { return (t.size() >= 2 && t[0] == '&'); }

    // Comparação
    bool operator==(const IDRota& ID) const { return t == ID.t; }
    bool operator!=(const IDRota& ID) const { return !operator==(ID); }

    // Impressão
    friend std::ostream& operator<<(std::ostream& X, const IDRota& ID) {
        return X << ID.t;
    }
};

/* *************************
   * CLASSE PONTO          *
   ************************* */

/// Um ponto no mapa
struct Ponto
{
    IDPonto id;          // Identificador do ponto
    std::string nome;    // Denominação usual do ponto
    double latitude;     // Em graus: -90 polo sul, +90 polo norte
    double longitude;    // Em graus: de -180 a +180

    // Construtor padrão
    Ponto() : id(), nome(""), latitude(0.0), longitude(0.0) {}

    // Teste de validade
    bool valid() const { return id.valid(); }

    // Sobrecarga de operadores para comparação
    bool operator==(const Ponto& outro) const { return id == outro.id; }
    bool operator!=(const Ponto& outro) const { return !(*this == outro); }
};

/* *************************
   * FUNCAO HAVERSINE      *
   ************************* */

/// Distância entre 2 pontos (fórmula de Haversine)
double haversine(const Ponto& P1, const Ponto& P2) {
    double lat1 = P1.latitude * PI / 180.0;
    double lon1 = P1.longitude * PI / 180.0;
    double lat2 = P2.latitude * PI / 180.0;
    double lon2 = P2.longitude * PI / 180.0;

    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    double a = std::sin(dlat / 2) * std::sin(dlat / 2) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dlon / 2) * std::sin(dlon / 2);

    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return EARTH_RADIUS_KM * c;
}

/* *************************
   * CLASSE ROTA           *
   ************************* */

/// Uma rota no mapa
struct Rota
{
    IDRota id;               // Identificador da rota
    std::string nome;        // Denominação usual da rota
    IDPonto extremidade[2];  // Ids dos pontos extremos da rota
    double comprimento;      // Comprimento da rota (em km)

    // Construtor padrão
    Rota() : id(), nome(""), comprimento(0.0) {}

    // Teste de validade
    bool valid() const { return id.valid(); }

    // Sobrecarga de operadores para comparação
    bool operator==(const Rota& outra) const { return id == outra.id; }
    bool operator!=(const Rota& outra) const { return !(*this == outra); }
};

/* *************************
   * CLASSE CAMINHO        *
   ************************* */

/// Um caminho encontrado entre dois pontos: uma lista de pares <IDRota,IDPonto>
using Caminho = std::list<std::pair<IDRota, IDPonto>>;

/* *************************
   * CLASSE PLANEJADOR     *
   ************************* */

/// A classe que armazena os pontos e as rotas do mapa do Planejador
/// e calcula caminho mais curto entre pontos.
class Planejador
{
private:
    std::list<Ponto> pontos;
    std::list<Rota> rotas;

public:
    /// Cria um mapa vazio
    Planejador() : pontos(), rotas() {}

    /// Cria um mapa com o conteúdo dos arquivos arq_pontos e arq_rotas
    Planejador(const std::string& arq_pontos,
               const std::string& arq_rotas) : Planejador()
    {
        ler(arq_pontos, arq_rotas);
    }

    /// Destrutor
    ~Planejador() { clear(); }

    /// Torna o mapa vazio
    void clear() { pontos.clear(); rotas.clear(); }

    /// Testa se o mapa está vazio
    bool empty() const { return pontos.empty(); }

    /// Retorna um Ponto do mapa, passando a id como parâmetro.
    Ponto getPonto(const IDPonto& Id) const;

    /// Retorna uma Rota do mapa, passando a id como parâmetro.
    Rota getRota(const IDRota& Id) const;

    /// Imprime o mapa no console
    void imprimirPontos() const;
    void imprimirRotas() const;

    /// Lê um mapa dos arquivos arq_pontos e arq_rotas.
    bool ler(const std::string& arq_pontos, const std::string& arq_rotas);

    /// Calcula o caminho mais curto no mapa entre origem e destino, usando o algoritmo A*
    double calculaCaminho(const IDPonto& id_origem,
                          const IDPonto& id_destino,
                          Caminho& C, int& NA, int& NF);
};

#endif // _PLANEJADOR_H_
