-Condição de parada por profundidade; (DONE)
-Dado um nó e um vértice, buscar os nós que tenham o vértice em comum; (PART. DONE)
--Pegar os nós em todos os níveis, e não apenas nas folhas.
-Buscar a lista de nós não conformes em um nível; (DONE)
--Nós não conformes são aqueles que possuem algum vizinho que não esteja na
  mesma profundidade;
--Alterar a função Split para receber um parâmetro de orientação do padrão
  desejado (pro caso de 3 filhos) ou criar os 4 filhos normalmente;

-Depois:
--Criar uma malha com os vértices + arestas;
---Malha deve ser independente da árvore.
---Usar uma estrutura tipo CHE?
