# Implementação do jogo Boulder Dash
Esse projeto foi feito para a disciplina de Programação 2, a partir da biblioteca para implementação de gogos gráficos Allegro 5, com a linguagem C. 

## Compilação 
Compila a partir do C, utilizando comandos make.  
Para efetuar a compilação, basta ir ao diretório src e digitar make.
Para excluir todos os arquivos temporários digite make clean,
e para excluir temporários e o executável digite make purge.  
Ao compilar criará um arquivo executável com o nome Boulder_Dash.

## Execução 
Para executar o jogo basta digitar ./Boulder_Dash no diretório com o jogo compilado. 
A movimentação do jogador é efetuadas através das teclas (up,down, right,left) e/ou (w,s,d,a).
É possível transitar entre os níveis, clicando nos botões de page up e page dow no jogo. Ao efetuar novamente um nível completo em uma mesma sessão de jogo, a sua pontuação no level será substituída pelo score da partida mais recente. 

## Highscores
As melhores pontuações são salvas e é possível visualizá-las no final da partida. A pontuação consiste em 
pontuação = 10* _nº diamantes coletados_+2*_tempo restante_

## Easter Egg
Todo mundo que já jogou Minecraft sabe como era frustrante quando você ficava horas construindo algo e um creeper chegava perto de você e destruia tudo.
Então, visando limpar a imagem dos Creepers, nesse jogo resolvi que o creeper ajuda o jogador de uma maneira explosiva. 
Para tal, no canto superior direito da tela tem um ícone com um creeper, ao clicar nesse ícone, todas as pedras do nível são destruídas, porém cuidado, caso todas as pedras sejam destruídas, os monstros viverão livremente sem perigos. 

## Autor 
Rubens Zandomenighi Laszlo
