#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// FEITO POR HUMBERTO LIMA, RA 848829
// CÓDIGO É BASEADO NO USO DO FORK() PARA CRIAÇÃO DE UM PROCESSO FILHO E KILL() PARA ENVIAR SINAIS PARA O MESMO,
// ALÉM DE UMA FUNÇÃO CHAMADA SIGNAL_HANDLER, QUE RECEBE OS DEVIDOS SINAIS E FAZ PRINTS

// TODOS OS SLEEPS SÃO UTILIZADOS PARA FACILIDADE DE LEITURA DOS PRINTS E SINCRONIZAÇÃO DAS AÇÕES ENTRE OS PROCESSOS,
// SEM ELES TUDO ACONTECE MUITO RÁPIDO E AS AÇÕES SÃO PRINTADAS FORA DE ORDEM

// Função signal_handler é utilizada para receber os sinais, enviados pelo processo pai, no processo filho e printar para facilitar o entendimento do código
// da maneira que está, só consegue lidar com os dois sinais utilizados

// SIGCONT (para resumir um processo pausado), e
// SIGTERM (instrui o processo a terminar graciosamente,
// dando a ele a oportunidade de finalizar suas operações e liberar recursos de forma ordenada antes de encerrar)

void signal_handler(int sig) {
  if (sig == SIGCONT) {
    sleep(1);
    printf("CHILD (PID: %d) recebeu SIGCONT, resumindo...\n", getpid());
  } else if (sig == SIGTERM) {
    sleep(1);
    printf("CHILD (PID: %d) recebeu SIGTERM, encerrando...\n", getpid());
    exit(0); // Encerra child process
  }
}

int main() {
  // Utiliza fork() para criar um novo processo. O retorno da função fork()
  // será o PID (identificador de processo) do novo processo no processo pai,
  // e 0 no processo filho. Se houver um erro, o retorno será -1.
  pid_t pid = fork();

  if (pid == 0) {
    // Processo CHILD

    // Inicializa o signal handler
    signal(SIGCONT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("CHILD (PID: %d) em execução...\n", getpid());

    sleep(1);

    printf("CHILD (PID: %d) entrando em estado de pausa...\n", getpid());

    // Processo child entra em estado de pausa durante o tempo informado
    pause();
    sleep(100);

    // Processo child encerra antes de receber SIGTERM
    printf("CHILD (PID: %d) encerrando ANTES DE RECEBER SIGTERM...\n", getpid());
    exit(0);

  } else if (pid > 0) {
    // Processo PARENT

    printf("PARENT (PID: %d) em execução...\n", getpid());

    sleep(2); // Espera child exibir sua mensagem

    printf("PARENT (PID: %d) aguardando 5 segundos para enviar SIGCONT para child...\n", getpid());
    sleep(5); // Espera 5 segundos antes de enviar o sinal SIGCONT para o processo child

    printf("PARENT (PID: %d) enviando SIGCONT para child...\n", getpid());
    kill(pid, SIGCONT); // Processo parent envia sinal SIGCONT para o processo child, retomando-o

    sleep(2); // Espera child exibir sua mensagem

    printf("PARENT (PID: %d) aguardando 5 segundos para enviar SIGTERM para child...\n", getpid());
    sleep(5); // Espera 5 segundos antes de enviar o sinal SIGTERM para o processo child

    printf("PARENT (PID: %d) enviando SIGTERM para child...\n", getpid());
    kill(pid, SIGTERM); // Processo parent envia sinal SIGTERM para o processo child, terminando-o

    // Processo parent espera que o processo child termine
    // Isso é importante para garantir que o processo parent não termine antes do processo child
    
    wait(NULL);
  } else {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  return 0;
}
