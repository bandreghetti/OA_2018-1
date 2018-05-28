Aluno: Bruno Andreghetti Dantas         Matrícula: 15/0007060
Aluna: Sara Gomes Cardoso               Matrícula: 15/0021712
---

##Comando geral

*Para compilar todos os programas em Ansi C e gerar os arquivos executáveis:

`./build.sh`

##Comandos relacionados à lista 1

*Para gerar arquivo de índices primários e secundários da `lista1.txt`:

```
$ ./idxgen lista1.txt
```

Serão gerados `indicelista1.ind`, `indiceseclista1.ind` e `lista1.data`,
arquivos binários que podem ter seu conteúdo visualizado no terminal.

*Para visualizar a lista de índices primários, deve-se usar o seguinte 
comando no terminal:

```
$ ./viewprimidx lista1.data
```

*Para visualizar a lista de índices secundários, deve-se usar o seguinte 
comando no terminal:

```
.$ /viewsecidx lista1.data
```

*Para inserir novos registros de alunos em 'lista1.data':

```
.$ /addreg lista1.data
```

*Será adicionado um novo registro em `lista1.data`, `indicelista1.ind`, 
`indiceseclista1.ind` e em `indicesecinvlista1.ind`, podendo-se 
visualizar o resultado no terminal com o comando:

```
$ ./viewdata lista1.data
```

*Para visualizar a adição do registro na lista de índices primários, deve-se 
usar o seguinte comando no terminal:

```
$ ./viewprimidx lista1.data
```

*Para visualizar a adição do registro na lista de índices secundários, deve-se
usar o seguinte comando no terminal:

```
$ ./viewsecidx lista1.data
```

O usuário deve representar o campo matrícula com até 6 caracteres, os campos OP 
e curso, com até 2 caracteres e o campo turma, com até 1 caractere.
Caso os campos do registro adicionado contenham mais caracteres do que o 
esperado, estes serão truncados em `lista1.data`.

*Para excluir registros de alunos em `lista1.data`:

```
$ ./delreg lista1.data
```

Será excluído dos arquivos `indicelista1.ind`, `indiceseclista1.ind`, 
`indicesecinvlista1.ind` e `lista1.data` o registro escolhido pelo usuário.

*Para visualizar a remoção na lista de índices primários, deve-se utilizar o 
seguinte comando no terminal:

```
$ ./viewprimidx lista1.data
```

*Para visualizar a remoção na lista de índices secundários, deve-se utilizar o 
seguinte comando no terminal:

```
$ ./viewsecidx lista1.data
```

*Para visualizar a remoção na lista, com todos os campos explícitos, deve-se 
utilizar o seguinte comando no terminal:

```
$ ./viewdata lista1.data
```

Caso o registro não exista, será avisado ao usuário por meio de uma mensagem no 
terminal.

##Comandos relacionados a lista 2

*Para gerar arquivo de índices primários e secundários da `lista2.txt`:

```
$ ./idxgen lista2.txt
```

Serão gerados `indicelista2.ind`, `indiceseclista2.ind` e `lista2.data`, 
arquivos binários que poderão ter seu conteúdo visualizado no terminal.

*Para visualizar a lista de índices primários, deve-se usar o seguinte comando
no terminal:

```
$ ./viewprimidx lista2.data
```

*Para visualizar a lista de índices secundários, deve-se usar o seguinte 
comando no terminal:

```
$ ./viewsecidx lista2.data
```

*Para inserir novos registros de alunos em `lista2.data`:

```
$ ./addreg lista2.data
```

*Será adicionado um novo registro em `lista2.data`,`indicelista2.ind`,
`indiceseclista2.ind` e em `indicesecinvlista2.ind`, podendo-se visualizar o
resultado no terminal com os comandos:

```
$ ./viewdata lista2.data
```

*Para visualizar a adição do registro na lista de índices primários, deve-se 
usar o seguinte comando no terminal:

```
$ ./viewprimidx lista2.data
```

*Para visualizar a adição do registro na lista de índices secundários, deve-se
usar o seguinte comando no terminal:

```
$ ./viewsecidx lista2.data
```

O usuário deve representar o campo matrícula com até 6 caracteres, os campos OP
e curso, com até 2 caracteres e o campo turma, com até 1 caractere.
Caso os campos do registro adicionado contenham mais caracteres do que o 
esperado, estes serão truncados em `lista2.data`.

*Para excluir registros de alunos em `lista2.data`:

```
$ ./delreg lista2.data
```

Será exlcuído dos arquivos `indicelista2.ind`, `indiceseclista2.ind`, 
`indicesecinvlista2.ind` e `lista2.data` o registro escolhido pelo usuário.

*Para visualizar a remoção na lista de índices primários, deve-se utilizar o 
seguinte comando no terminal:

```
$ ./viewprimidx lista2.data
```

*Para visualizar a remoção na lista de índices secundários, deve-se utilizar o 
seguinte comando no terminal:

```
$ ./viewsecidx lista2.data
```

*Para visualizar a remoção na lista, com todos os campos explícitos, deve-se 
utilizar o seguinte comando no terminal:

```
$ ./viewdata lista2.data
```

Caso o registro não exista, será avisado ao usuário por meio de uma mensagem no
terminal.

#Comandos relacionados às listas 1 e 2

*Unir `lista1.data` e `lista2.data`: 

```
$ ./mergefiles lista1.data lista2.data
```

*Serão gerados `indicelista12.ind`, `indicesecinvlista12.ind` e `lista12.data`, 
o resultado poderá ser observado no terminal com o seguinte comando:

```
$ ./viewdata lista12.data
```

*Para visualizar o a união das listas de índices primários:

```
$ ./viewprimidx lista12.data
```

*Para visualizar o a união das listas de índices secundários:

```
$ ./viewsecidx lista12.data
```