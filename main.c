#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_USERS 10
#define MAX_TRANSACOES 100

// Estrutura para armazenar transações
typedef struct {
    char data[20];
    char tipoOperacao[10]; // "Compra" ou "Venda"
    char criptomoeda[20];
    float valor;
    float taxa;
} Transacao;

// Estrutura para armazenar dados do usuário
typedef struct {
    char cpf[12];
    char senha[20];
    float saldoReais;
    float saldoBitcoin;
    float saldoEthereum;
    float saldoRipple;
    Transacao transacoes[MAX_TRANSACOES];
    int numTransacoes; // Número atual de transações
} Usuario;

// Funções
void cadastrarUsuario(Usuario usuarios[], int *totalUsuarios);
int efetuarLogin(Usuario usuarios[], int totalUsuarios, char cpf[], char senha[]);
void consultarSaldo(Usuario *usuario);
void depositarReais(Usuario *usuario, float valor);
int sacarReais(Usuario *usuario, float valor, char senha[]);
void comprarCriptomoeda(Usuario *usuario, char senha[], char criptomoeda[], float valor, float taxa);
void venderCriptomoeda(Usuario *usuario, char senha[], char criptomoeda[], float valor, float taxa);
void salvarDados(Usuario usuarios[], int totalUsuarios);
void carregarDados(Usuario usuarios[], int *totalUsuarios);
void atualizarCotacao(float *cotacaoBitcoin, float *cotacaoEthereum, float *cotacaoRipple);
void salvarExtrato(Usuario usuario);
void adicionarTransacao(Usuario *usuario, Transacao novaTransacao);
void consultarHistorico(Usuario usuario);
void obterDataAtual(char *data);

int main() {
    Usuario usuarios[MAX_USERS];
    int totalUsuarios = 0;
    carregarDados(usuarios, &totalUsuarios);

    float cotacaoBitcoin = 50000.0, cotacaoEthereum = 3000.0, cotacaoRipple = 1.0;
    int opcao, usuarioLogado = -1;
    char cpf[12], senha[20];
    float valor;

    while (1) {
        printf("\n--- Exchange de Criptomoedas ---\n");
        if (usuarioLogado == -1) {
            printf("1. Efetuar Login\n");
            printf("2. Cadastrar Novo Usuário\n");
        } else {
            printf("1. Consultar Saldo\n");
            printf("2. Depositar Reais\n");
            printf("3. Sacar Reais\n");
            printf("4. Comprar Criptomoeda\n");
            printf("5. Vender Criptomoeda\n");
            printf("6. Consultar Histórico de Transações\n");
            printf("7. Atualizar Cotação\n");
            printf("8. Sair\n");
        }
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        if (usuarioLogado == -1 && opcao == 1) {
            printf("CPF: ");
            scanf("%s", cpf);
            printf("Senha: ");
            scanf("%s", senha);
            usuarioLogado = efetuarLogin(usuarios, totalUsuarios, cpf, senha);
        } else if (usuarioLogado == -1 && opcao == 2) {
            cadastrarUsuario(usuarios, &totalUsuarios);
            salvarDados(usuarios, totalUsuarios);
        } else if (usuarioLogado != -1) {
            switch (opcao) {
                case 1:
                    consultarSaldo(&usuarios[usuarioLogado]);
                    break;
                case 2:
                    printf("Valor para depositar: ");
                    scanf("%f", &valor);
                    depositarReais(&usuarios[usuarioLogado], valor);
                    break;
                case 3:
                    printf("Valor para sacar: ");
                    scanf("%f", &valor);
                    printf("Senha: ");
                    scanf("%s", senha);
                    sacarReais(&usuarios[usuarioLogado], valor, senha);
                    break;
                case 4:
                    printf("Criptomoeda (Bitcoin/Ethereum/Ripple): ");
                    char criptomoeda[10];
                    scanf("%s", criptomoeda);
                    printf("Valor para compra: ");
                    scanf("%f", &valor);
                    printf("Senha: ");
                    scanf("%s", senha);
                    float taxaCompra;
                    if (strcmp(criptomoeda, "Bitcoin") == 0) taxaCompra = 0.02;
                    else if (strcmp(criptomoeda, "Ethereum") == 0) taxaCompra = 0.01;
                    else taxaCompra = 0.01;
                    comprarCriptomoeda(&usuarios[usuarioLogado], senha, criptomoeda, valor, taxaCompra);
                    break;
                case 5:
                    printf("Criptomoeda (Bitcoin/Ethereum/Ripple): ");
                    scanf("%s", criptomoeda);
                    printf("Valor para venda: ");
                    scanf("%f", &valor);
                    printf("Senha: ");
                    scanf("%s", senha);
                    float taxaVenda;
                    if (strcmp(criptomoeda, "Bitcoin") == 0) taxaVenda = 0.03;
                    else if (strcmp(criptomoeda, "Ethereum") == 0) taxaVenda = 0.02;
                    else taxaVenda = 0.01;
                    venderCriptomoeda(&usuarios[usuarioLogado], senha, criptomoeda, valor, taxaVenda);
                    break;
                case 6:
                    consultarHistorico(usuarios[usuarioLogado]);
                    break;
                case 7:
                    atualizarCotacao(&cotacaoBitcoin, &cotacaoEthereum, &cotacaoRipple);
                    printf("Nova cotação Bitcoin: %.2f\n", cotacaoBitcoin);
                    printf("Nova cotação Ethereum: %.2f\n", cotacaoEthereum);
                    printf("Nova cotação Ripple: %.2f\n", cotacaoRipple);
                    break;
                case 8:
                    salvarDados(usuarios, totalUsuarios);
                    salvarExtrato(usuarios[usuarioLogado]);
                    usuarioLogado = -1;
                    printf("Deslogado com sucesso!\n");
                    break;
                default:
                    printf("Opção inválida!\n");
                    break;
            }
        } else {
            printf("Opção inválida!\n");
        }
    }

    return 0;
}

// Implementação das funções

void cadastrarUsuario(Usuario usuarios[], int *totalUsuarios) {
    if (*totalUsuarios >= MAX_USERS) {
        printf("Limite de usuários atingido!\n");
        return;
    }

    Usuario novoUsuario;
    printf("Digite o CPF: ");
    scanf("%s", novoUsuario.cpf);

    // Verificar se o CPF já existe
    for (int i = 0; i < *totalUsuarios; i++) {
        if (strcmp(usuarios[i].cpf, novoUsuario.cpf) == 0) {
            printf("Este CPF já está cadastrado!\n");
            return;
        }
    }

    printf("Digite a senha: ");
    scanf("%s", novoUsuario.senha);

    // Inicializa os saldos e transações
    novoUsuario.saldoReais = 0.0;
    novoUsuario.saldoBitcoin = 0.0;
    novoUsuario.saldoEthereum = 0.0;
    novoUsuario.saldoRipple = 0.0;
    novoUsuario.numTransacoes = 0;

    // Adiciona o novo usuário ao array de usuários
    usuarios[*totalUsuarios] = novoUsuario;
    (*totalUsuarios)++;
    printf("Cadastro realizado com sucesso!\n");
}

int efetuarLogin(Usuario usuarios[], int totalUsuarios, char cpf[], char senha[]) {
    for (int i = 0; i < totalUsuarios; i++) {
        if (strcmp(usuarios[i].cpf, cpf) == 0 && strcmp(usuarios[i].senha, senha) == 0) {
            printf("Login realizado com sucesso!\n");
            return i; // Retorna o índice do usuário
        }
    }
    printf("CPF ou senha inválidos.\n");
    return -1;
}

void consultarSaldo(Usuario *usuario) {
    printf("Saldo em Reais: %.2f\n", usuario->saldoReais);
    printf("Saldo em Bitcoin: %.6f\n", usuario->saldoBitcoin);
    printf("Saldo em Ethereum: %.6f\n", usuario->saldoEthereum);
    printf("Saldo em Ripple: %.6f\n", usuario->saldoRipple);
}

void depositarReais(Usuario *usuario, float valor) {
    if (valor > 0) {
        usuario->saldoReais += valor;
        printf("Depósito realizado com sucesso. Novo saldo em Reais: %.2f\n", usuario->saldoReais);
    } else {
        printf("Valor inválido!\n");
    }
}

int sacarReais(Usuario *usuario, float valor, char senha[]) {
    if (strcmp(usuario->senha, senha) != 0) {
        printf("Senha inválida!\n");
        return 0;
    }

    if (valor <= usuario->saldoReais) {
        usuario->saldoReais -= valor;
        printf("Saque realizado com sucesso. Novo saldo em Reais: %.2f\n", usuario->saldoReais);
        return 1;
    } else {
        printf("Saldo insuficiente!\n");
        return 0;
    }
}

void comprarCriptomoeda(Usuario *usuario, char senha[], char criptomoeda[], float valor, float taxa) {
    if (strcmp(usuario->senha, senha) != 0) {
        printf("Senha inválida!\n");
        return;
    }

    float valorTotal = valor * (1 + taxa);
    if (valor <= usuario->saldoReais) {
        usuario->saldoReais -= valorTotal;

        if (strcmp(criptomoeda, "Bitcoin") == 0) {
            usuario->saldoBitcoin += valor;
        } else if (strcmp(criptomoeda, "Ethereum") == 0) {
            usuario->saldoEthereum += valor;
        } else if (strcmp(criptomoeda, "Ripple") == 0) {
            usuario->saldoRipple += valor;
        }

        Transacao novaTransacao;
        obterDataAtual(novaTransacao.data);
        strcpy(novaTransacao.tipoOperacao, "Compra");
        strcpy(novaTransacao.criptomoeda, criptomoeda);
        novaTransacao.valor = valor;
        novaTransacao.taxa = taxa;

        adicionarTransacao(usuario, novaTransacao);
        printf("Compra realizada com sucesso!\n");
    } else {
        printf("Saldo insuficiente!\n");
    }
}

void venderCriptomoeda(Usuario *usuario, char senha[], char criptomoeda[], float valor, float taxa) {
    if (strcmp(usuario->senha, senha) != 0) {
        printf("Senha inválida!\n");
        return;
    }

    if ((strcmp(criptomoeda, "Bitcoin") == 0 && valor <= usuario->saldoBitcoin) ||
        (strcmp(criptomoeda, "Ethereum") == 0 && valor <= usuario->saldoEthereum) ||
        (strcmp(criptomoeda, "Ripple") == 0 && valor <= usuario->saldoRipple)) {

        usuario->saldoReais += valor * (1 - taxa);

        if (strcmp(criptomoeda, "Bitcoin") == 0) {
            usuario->saldoBitcoin -= valor;
        } else if (strcmp(criptomoeda, "Ethereum") == 0) {
            usuario->saldoEthereum -= valor;
        } else if (strcmp(criptomoeda, "Ripple") == 0) {
            usuario->saldoRipple -= valor;
        }

        Transacao novaTransacao;
        obterDataAtual(novaTransacao.data);
        strcpy(novaTransacao.tipoOperacao, "Venda");
        strcpy(novaTransacao.criptomoeda, criptomoeda);
        novaTransacao.valor = valor;
        novaTransacao.taxa = taxa;

        adicionarTransacao(usuario, novaTransacao);
        printf("Venda realizada com sucesso!\n");
    } else {
        printf("Saldo insuficiente!\n");
    }
}

void salvarDados(Usuario usuarios[], int totalUsuarios) {
    FILE *file = fopen("usuarios.dat", "wb");
    fwrite(usuarios, sizeof(Usuario), totalUsuarios, file);
    fclose(file);
}

void carregarDados(Usuario usuarios[], int *totalUsuarios) {
    FILE *file = fopen("usuarios.dat", "rb");
    if (file != NULL) {
        *totalUsuarios = fread(usuarios, sizeof(Usuario), MAX_USERS, file);
        fclose(file);
    }
}

void atualizarCotacao(float *cotacaoBitcoin, float *cotacaoEthereum, float *cotacaoRipple) {
    // Simulação de atualização de cotação (pode ser substituído por uma chamada a uma API)
    *cotacaoBitcoin += (*cotacaoBitcoin * 0.01); // Aumenta 1%
    *cotacaoEthereum += (*cotacaoEthereum * 0.01); // Aumenta 1%
    *cotacaoRipple += (*cotacaoRipple * 0.01); // Aumenta 1%
}

void salvarExtrato(Usuario usuario) {
    FILE *file = fopen("extrato.txt", "a");
    fprintf(file, "Extrato do usuário %s:\n", usuario.cpf);
    for (int i = 0; i < usuario.numTransacoes; i++) {
        fprintf(file, "%s - %s: %.2f (Taxa: %.2f)\n",
                usuario.transacoes[i].data,
                usuario.transacoes[i].tipoOperacao,
                usuario.transacoes[i].valor,
                usuario.transacoes[i].taxa);
    }
    fprintf(file, "Saldo em Reais: %.2f\n", usuario.saldoReais);
    fprintf(file, "Saldo em Bitcoin: %.6f\n", usuario.saldoBitcoin);
    fprintf(file, "Saldo em Ethereum: %.6f\n", usuario.saldoEthereum);
    fprintf(file, "Saldo em Ripple: %.6f\n", usuario.saldoRipple);
    fclose(file);
}

void adicionarTransacao(Usuario *usuario, Transacao novaTransacao) {
    if (usuario->numTransacoes < MAX_TRANSACOES) {
        usuario->transacoes[usuario->numTransacoes] = novaTransacao;
        usuario->numTransacoes++;
    } else {
        printf("Limite de transações atingido!\n");
    }
}

void consultarHistorico(Usuario usuario) {
    printf("Histórico de Transações:\n");
    for (int i = 0; i < usuario.numTransacoes; i++) {
        printf("%s - %s: %.2f (Taxa: %.2f)\n",
               usuario.transacoes[i].data,
               usuario.transacoes[i].tipoOperacao,
               usuario.transacoes[i].valor,
               usuario.transacoes[i].taxa);
    }
}

void obterDataAtual(char *data) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(data, "%02d/%02d/%04d %02d:%02d:%02d",
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
}