#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_USERS 10
#define MAX_TRANSACOES 100

// Estrutura para armazenar dados do usuário
typedef struct {
    char cpf[12];
    char senha[20];
    float saldoReais;
    float saldoBitcoin;
    float saldoEthereum;
    float saldoRipple;
} Usuario;

// Estrutura para armazenar uma transação
typedef struct {
    char data[20]; // data da transação
    char tipo[20]; // tipo de operação (depósito, saque, compra, venda)
    char descricao[50]; // detalhes da transação
    float valor; // valor da transação
    float taxa; // taxa envolvida
} Transacao;

// Estrutura para a carteira de um usuário, incluindo o histórico de transações
typedef struct {
    Usuario usuario;
    Transacao transacoes[MAX_TRANSACOES];
    int totalTransacoes;
} Carteira;

// Função para adicionar uma transação ao histórico
void adicionarTransacao(Carteira *carteira, const char *tipo, const char *descricao, float valor, float taxa) {
    if (carteira->totalTransacoes < MAX_TRANSACOES) {
        Transacao novaTransacao;

        // Obtém a data atual
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        strftime(novaTransacao.data, 20, "%Y-%m-%d %H:%M:%S", tm_info);

        strcpy(novaTransacao.tipo, tipo);
        strcpy(novaTransacao.descricao, descricao);
        novaTransacao.valor = valor;
        novaTransacao.taxa = taxa;

        // Adiciona a transação à carteira
        carteira->transacoes[carteira->totalTransacoes] = novaTransacao;
        carteira->totalTransacoes++;
    } else {
        printf("Limite de transações atingido!\n");
    }
}

// Função para consultar extrato e salvar em arquivo texto
void consultarExtrato(Carteira *carteira) {
    FILE *file = fopen("extrato.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo 'extrato.txt' para salvar o extrato!\n");
        return;
    }

    fprintf(file, "Extrato do CPF: %s\n", carteira->usuario.cpf);
    fprintf(file, "----------------------------------------\n");
    for (int i = 0; i < carteira->totalTransacoes; i++) {
        Transacao *transacao = &carteira->transacoes[i];
        fprintf(file, "Data: %s | Tipo: %s | Descrição: %s | Valor: %.2f | Taxa: %.2f\n", 
                transacao->data, transacao->tipo, transacao->descricao, transacao->valor, transacao->taxa);
    }

    fclose(file);
    printf("Extrato salvo em 'extrato.txt'.\n");
}

// Função para salvar os dados das carteiras em arquivo binário
void salvarCarteiras(Carteira carteiras[], int totalCarteiras) {
    FILE *file = fopen("carteiras.dat", "wb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo 'carteiras.dat' para salvar as carteiras!\n");
        return;
    }
    fwrite(carteiras, sizeof(Carteira), totalCarteiras, file);
    fclose(file);
}

// Função para carregar os dados das carteiras de um arquivo binário
void carregarCarteiras(Carteira carteiras[], int *totalCarteiras) {
    FILE *file = fopen("carteiras.dat", "rb");
    if (file != NULL) {
        *totalCarteiras = fread(carteiras, sizeof(Carteira), MAX_USERS, file);
        fclose(file);
    } else {
        printf("Erro ao abrir o arquivo 'carteiras.dat' para carregar as carteiras!\n");
    }
}

// Função para realizar cadastro de novos usuários
void cadastrarUsuario(Carteira carteiras[], int *totalCarteiras) {
    if (*totalCarteiras >= MAX_USERS) {
        printf("Limite de usuários atingido!\n");
        return;
    }

    Carteira novaCarteira;
    Usuario *novoUsuario = &novaCarteira.usuario;

    printf("Digite o CPF (até 11 dígitos): ");
    scanf("%11s", novoUsuario->cpf);

    // Verificar se o CPF já existe
    for (int i = 0; i < *totalCarteiras; i++) {
        if (strcmp(carteiras[i].usuario.cpf, novoUsuario->cpf) == 0) {
            printf("Este CPF já está cadastrado!\n");
            return;
        }
    }

    printf("Digite a senha (até 19 caracteres): ");
    scanf("%19s", novoUsuario->senha);

    // Inicializa os saldos
    novoUsuario->saldoReais = 0.0;
    novoUsuario->saldoBitcoin = 0.0;
    novoUsuario->saldoEthereum = 0.0;
    novoUsuario->saldoRipple = 0.0;

    // Inicializa o histórico de transações
    novaCarteira.totalTransacoes = 0;

    // Adiciona a nova carteira ao array de carteiras
    carteiras[*totalCarteiras] = novaCarteira;
    (*totalCarteiras)++;

    printf("Cadastro realizado com sucesso!\n");
    salvarCarteiras(carteiras, *totalCarteiras);
}

int main() {
    Carteira carteiras[MAX_USERS];
    int totalCarteiras = 0;
    carregarCarteiras(carteiras, &totalCarteiras);

    // Aqui ficaria o restante do código do menu de opções e login/saída
    // Assim como as opções para realizar transações, chamadas das funções, etc.

    return 0;
}
