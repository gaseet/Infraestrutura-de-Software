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

    pause();
    sleep(1);

    // Processo CHILD encerra antes de receber SIGTERM
    printf("CHILD (PID: %d) encerrando ANTES DE RECEBER SIGTERM...\n", getpid());
    exit(0);

  } else if (pid > 0) {
    // Processo PARENT
    printf("PARENT (PID: %d) em execução...\n", getpid());
    sleep(2);

    printf("PARENT (PID: %d) aguardando 5 segundos para enviar SIGCONT para child...\n", getpid());
    sleep(5); // Espera 5 segundos antes de enviar o sinal SIGCONT para o processo filho

    printf("PARENT (PID: %d) enviando SIGCONT para child...\n", getpid());
    kill(pid, SIGCONT); // Processo pai envia sinal SIGCONT para o processo filho, retomando-o

    sleep(2);

    printf("PARENT (PID: %d) aguardando 5 segundos para enviar SIGTERM para child...\n", getpid());
    sleep(5); // Espera 5 segundos antes de enviar o sinal SIGTERM para o processo filho

    printf("PARENT (PID: %d) enviando SIGTERM para child...\n", getpid());
    kill(pid, SIGTERM); // Processo pai envia sinal SIGTERM para o processo filho, terminando-o
    //sleep(1);

    wait(NULL); // Processo pai espera que o processo filho termine
  } else {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  return 0;
}
