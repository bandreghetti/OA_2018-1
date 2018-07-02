Aluno: Bruno Andreghetti Dantas                 Matrícula: 15/0007060
Aluna: Sara Gomes Cardoso                       Matrícula: 15/0021712
---------------------------------------------------------------------

## Comando geral

* Inicialmente deve-se instalar o pacote libboost-dev inserindo o seguinte comando no terminal:

```
$ sudo apt install libboost-dev
```

* Para compilar o programa em C++ e gerar o arquivo executável:

```
$ g++ btreegen.cpp -o btreegen
```

*Para executar o programa compilado e gerar a árvore inicial com os registros contidos na lista.txt:
```
$ ./btreegen
```
## Para visualizar o conteúdo da árvore
Os nomes e matrículas foram concatenados para formar o índice primário que será visualizado na árvore implementada.
* O comando para visualização dos registros existentes na árvore é:

```
$ g++ btreeview.cpp -o btreeview
```
```
$ ./btreeview
```
Só foi possível inserir os registros na raiz da árvore. Houve a tentativa de inserir o restanto dos registros, mas a função split não correspondeu às expectativas.

## Para inserir um registro à árvore 
O usuário deve inserir um novo registro contendo os campos nome, com até 40 caracteres, matrícula, com até 5 caracteres, curso, com até 2 caracteres e turma, com até 1 caractere. 
Não foi possível implementar essa operação.

## Deletar registros da árvore
O usuário deve remover um registro já existente através da chave principal.
Não foi possível implementar essa operação.
