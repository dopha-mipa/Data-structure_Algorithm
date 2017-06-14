'''
# 2017-1 컴퓨터알고리즘 1202046 서나윤 / python3.5
'''
class tree_node :
  # profit : current status of profit
  # weight : current status of weight
  # ubd : upper bound of expected profit when search deeper
  def __init__ (self, profit, weight, ubd, depth, parentNode, rl) :
    self.profit = profit # 노드까지의 현재 이익
    self.weight = weight # 노드까지의 현재 무게
    self.ubd = ubd # 추후 탐색에 의해 기대되는 이익 상한
    self.depth = depth # 트리 레벨 = 해당하는 p_over_w 인덱스
    self.rl = rl # 부모의 왼쪽 자식이면 0, 오른쪽 자식이면 1. 루트는 -1
    self.parent = parentNode # 노드의 부모 노드
    self.children = [None, None] # 노드의 자식 노드

  def get_parent(self):
    return self.parent

  def get_left_child(self):
    return self.children[0]

  def get_right_child(self):
    return self.children[1]

  def set_left_child(self, node):
    self.children[0] = node

  def set_right_child(self, node):
    self.children[1] = node

  # 해당 노드 정보 출력
  def print_node(self):
    print("depth : ", self.depth, end='  ')
    print("profit : ", self.profit, " weight : ", self.weight, 
          " ubd : ", self.ubd)

  # 물건 갯수 N을 입력받아 knapsack의 벡터를 반환한다.
  # 단, 이때 출력되는 물건의 순서는 단위 무게당 이익이 큰 순서이다.
  def print_knapsack_vector(self, N):
    tree_result = [0 for i in range(N)]
    # TODO : depth를 이용한다. rl값이 -1이 아니면 계속한다.
    cur_node = self
    while (cur_node.rl != -1): # 루트까지 거슬러 올라간다
      if (cur_node.rl == 0):
        tree_result[cur_node.depth - 1] = 1
      cur_node = cur_node.get_parent()

    return tree_result