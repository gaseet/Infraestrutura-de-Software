#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void signal_handler(int sig) {
  if (sig == SIGCONT) {
    sleep(1);
    printf("CHILD (PID: %d) recebeu SIGCONT, resumindo...\n", getpid());
  } else if (sig == SIGTERM) {
    sleep(1);
    printf("CHILD (PID: %d) recebeu SIGTERM, encerrando...\n", getpid());
    exit(0);
  }
}

int main() {
  pid_t pid = fork();

  if (pid == 0) {
    // Processo CHILD

    // Inicializa o signal handler
    signal(SIGCONT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("CHILD (PID: %d) em execução...\n", getpid());

    sleep(1);

    printf("CHILD (PID: %d) entrando em estado de pausa...\n", getpid());

    // Processo child entra em estado de pausa infinitamente, até receber um sinal que o encerre
    while(1) {
        pause();
    }

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

    wait(NULL); // Processo parent espera que o processo child termine
  } else {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  return 0;
}
