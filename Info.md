**Submissão até dia 12/12/2025 às 23:59.**

Quando chegamos a certo ponto, é boa prática verificar se o programa corre no cluster Sigma.

*Não dar push* a binários nem object files!
Só deve estar no repositorio do git ficheiros do tipo `.c`,`.h`, `.md`(opcional) e uma `Makefile` (ou várias se necessário)

## Para fazer um commit
            "git pull"
            "git add <file1>, <file2>, etc." ou adicionar na barra lateral de Source Control
            "git status"
            "git commit -am "<message>"
            "git push"

## Para o Makefile
            O comando make clean deve limpar todos os ficheiros resultantes da compilação do projeto.
            Mais info no README.md

## Para aceder ao cluster Sigma:
        Tiago - "ssh ist1114151@cluster.rnl.tecnico.ulisboa.pt"
        Diogo - "ssh ist1113883@cluster.rnl.tecnico.ulisboa.pt"
        "ssh lab1p4"