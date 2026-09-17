//ALUNO 1: Vinícius Di Petta RA: 10769577
//ALUNO 2: Mariana Guimarães Lopes RA: 10738856
#include <stdio.h>
#include <stdlib.h>

#define MAX_AMOSTRAS 100

void inicializarMatrizes(
    float velocidades[][2],
    float sensores_frontais[][3],
    float sensores_laterais[][2],
    int quantidadeAmostras)
    {
        for (int i = 0; i < quantidadeAmostras; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                velocidades[i][j] = 40 + rand() % 81;
            }
        }
        for (int i = 0; i < quantidadeAmostras; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                sensores_frontais[i][j] =  1 + rand() % 150;
            }
        }
        for (int i = 0; i < quantidadeAmostras; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                sensores_laterais[i][j] = 0.5 + rand() % 6;
            }
        }
    }

void fusaoSensores(
    float sensores_frontais[][3],
    float processamento[][2],
    int quantidadeAmostras)
    {
        for (int i=0; i<quantidadeAmostras; i++)
        {
            float a = sensores_frontais[i][0];
            float b = sensores_frontais[i][1];
            float c = sensores_frontais[i][2];
            float mediana;
            if ((a <= b && b <= c) || (c <= b && b <= a))
            {
                mediana = b;
            }
            else if ((b <= a && a <= c) || (c <= a && a <= b))
            {
                mediana = a;
            }
            else
            {
                mediana = c;
            }
            processamento[i][0] = mediana;
        }

    }

void calculoDistanciaSegura(
    float velocidades[][2],
    float processamento[][2],
    float atrito,
    int sensibilidade,
    int quantidadeAmostras)
{
    float tempoReacao;

    if (sensibilidade == 1)
    {
        tempoReacao = 1.0;
    }
    else if (sensibilidade == 2)
    {
        tempoReacao = 1.5;
    }
    else
    {
        tempoReacao = 2.0;
    }

    for (int i = 0; i < quantidadeAmostras; i++)
    {
        float v = velocidades[i][0] / 3.6;

        processamento[i][1] = (v * tempoReacao) + (v * v) / (2 * atrito * 9.81);
    }
}
void aeb(
    float velocidades[][2], 
    float processamento[][2], 
    int status[][3],
    int quantidadeAmostras)
{
    for (int i = 0; i < quantidadeAmostras; i++)
    {
        float velocidadeRelativa = velocidades[i][0] - velocidades[i][1];

        if (velocidadeRelativa <= 0)
        {
            status[i][0] = 0;
        }
        else if (processamento[i][0] >= processamento[i][1])
        {
            status[i][0] = 0;
        }
        else if (processamento[i][0] >= processamento[i][1] * 0.5 && processamento[i][0] < processamento[i][1])
        {
            status[i][0] = 1;
        }
        else if (processamento[i][0] < processamento[i][1] * 0.5)
        {
            status[i][0] = 2;
        }
    }
}
void assistenteFaixa(
    float velocidades[][2],
    float sensores_laterais[][2], 
    int status[][3],
    int quantidadeAmostras)
{
    for (int i = 0; i < quantidadeAmostras; i++)
    {
        float margem = 0.5;

        if (velocidades[i][0] > 80)
        {
            margem += (velocidades[i][0] - 80) * 0.01;
        }
        //faixa esquerda
        if (sensores_laterais[i][0] < margem)
        {
            status[i][1] = 2;
        }
        else if (sensores_laterais[i][0] < margem + 0.20)
        {
            status[i][1] = 1;
        }
        else
        {
            status[i][1] = 0;
        }
        //faixa direita
        if (sensores_laterais[i][1] < margem)
        {
            status[i][2] = 2;
        }
        else if (sensores_laterais[i][1] < margem + 0.20)
        {
            status[i][2] = 1;
        }
        else
        {
            status[i][2] = 0;
        }
    }
}
void exibirRelatorio(
    float velocidades[][2],
    float sensores_frontais[][3],
    float sensores_laterais[][2],
    float processamento[][2],
    int status[][3],
    int quantidadeAmostras)
    {
        for (int i=0; i<quantidadeAmostras; i++)
        {
            printf("\nAmostra %d:\n", i+1);
            printf("Velocidade atual: %.2f km/h\n", velocidades[i][0]);
            printf("Velocidade do veículo à frente: %.2f km/h\n", velocidades[i][1]);
            printf("Radar: %.2f m\n", sensores_frontais[i][0]);
            printf("Lidar: %.2f m\n", sensores_frontais[i][1]);
            printf("Câmera: %.2f m\n", sensores_frontais[i][2]);
            printf("Distância da faixa esquerda: %.2f m\n", sensores_laterais[i][0]);
            printf("Distância da faixa direita: %.2f m\n", sensores_laterais[i][1]);
            printf("Distância validada: %.2f m\n", processamento[i][0]);
            printf("Distância segura: %.2f m\n", processamento[i][1]);
       
            if (status[i][0] == 0)
            {
                printf("Status frontal: SEGURO\n");
            }
            else if (status[i][0] == 1)
            {
                printf("Status frontal: ATENÇÃO\n");
            }
            else
            {
                printf("Status frontal: RISCO DE COLISÃO (AEB ACIONADO)\n");
            }
            if (status[i][1] == 0)
            {
                printf("Status faixa esquerda: NORMAL\n");
            }
            else if (status[i][1] == 1)
            {
                printf("Status faixa esquerda: ATENÇÃO\n");
            }
            else
            {
                printf("Status faixa esquerda: PERIGO DE INVASÃO\n");
            }
            if (status[i][2] == 0)
            {
                printf("Status faixa direita: NORMAL\n");
            }
            else if (status[i][2] == 1)
            {
                printf("Status faixa direita: ATENÇÃO\n");
            }
            else
            {
                printf("Status faixa direita: PERIGO DE INVASÃO\n");
            }
            if (status[i][0]==2 || status[i][1]==2 || status[i][2]==2)
            {
                printf("STATUS GERAL: INTERVENÇÃO CRÍTICA EXIGIDA\n");
            }
            else if (status[i][0]==1 || status[i][1]==1 || status[i][2]==1)
            {
                printf("STATUS GERAL: ATENÇÃO\n");
            }
            else
            {
                printf("STATUS GERAL: NORMAL\n");
            }
        }
    }

int main()
{
    float velocidades[MAX_AMOSTRAS][2]; // Coluna 0 (velocidade atual), Coluna 1 (Velocidade do veículo à frente).
    float sensores_frontais[MAX_AMOSTRAS][3]; // Coluna 0 (radar), Coluna 1 (lidar), Coluna 2 (câmera)
    float sensores_laterais[MAX_AMOSTRAS][2]; // Coluna 0 (distância da faixa esquerda), Coluna 1 (distância da faixa direita).
    float processamento[MAX_AMOSTRAS][2]; // Coluna 0 (distância validada), Coluna 1 (distância segura calculada).
    int status[MAX_AMOSTRAS][3]; // Coluna 0 (status frontal), Coluna 1 (faixa esquerda), Coluna 2 (faixa direita).

    float atrito;
    int sensibilidade;

    int opcao;
    int quantidadeAmostras = 0;

    printf("Digite o valor do atrito da via: ");
    scanf("%f", &atrito);
    printf("Digite o nível de sensibilidade do ADAS (1 - esportivo, 2 - normal ou 3 - seguro): ");
    scanf("%d", &sensibilidade);
    do 
    {
        printf("\nMenu de opções:\n");
        printf("1 - Carregar dados iniciais\n");
        printf("2 - Inserir nova amostra\n");
        printf("3 - Processar e exibir relatório de riscos\n");
        printf("4 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
            case 1:
                inicializarMatrizes(velocidades, sensores_frontais, sensores_laterais, quantidadeAmostras);
                quantidadeAmostras = 50;
                printf("\nMatrizes inicializadas com sucesso!\n");
                break;
            case 2:
                if (quantidadeAmostras >= MAX_AMOSTRAS)
                {
                    printf("\nLimite de amostras atingido!\n");
                    break;
                }
                else
                {
                    int qtd = quantidadeAmostras;

                    printf("\nVelocidade atual: ");
                    scanf("%f", &velocidades[qtd][0]);
                    printf("\nVelocidade do veiculo da frente: ");
                    scanf("%f", &velocidades[qtd][1]);
                    printf("\nRadar: ");
                    scanf("%f", &sensores_frontais[qtd][0]);
                    printf("\nLidar: ");
                    scanf("%f", &sensores_frontais[qtd][1]);
                    printf("\nCamera: ");
                    scanf("%f", &sensores_frontais[qtd][2]);
                    printf("\nDistancia da faixa da esquerda: ");
                    scanf("%f", &sensores_laterais[qtd][0]);
                    printf("\nDistancia da faixa da direita: ");
                    scanf("%f", &sensores_laterais[qtd][1]);

                    quantidadeAmostras++;

                    printf("\nInserida nova amostra!\n");
                    break;
                }
            case 3:
                fusaoSensores(sensores_frontais, processamento, quantidadeAmostras);
                calculoDistanciaSegura(velocidades, processamento, atrito, sensibilidade, quantidadeAmostras);
                aeb(velocidades, processamento, status, quantidadeAmostras);
                assistenteFaixa(velocidades, sensores_laterais, status, quantidadeAmostras);
                exibirRelatorio(velocidades, sensores_frontais, sensores_laterais, processamento, status, quantidadeAmostras);
                break;
            case 4:
                printf("\nEncerrando simulador...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 4);
    return 0;
}