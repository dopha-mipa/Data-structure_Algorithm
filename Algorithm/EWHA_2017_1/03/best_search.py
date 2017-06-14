'''
# 2017-1 컴퓨터알고리즘 1202046 서나윤 / python3.5
# 0-1 Knapsack with best search
'''
import tree_node as tn # 트리 탐색을 위한 클래스
import datetime as dt

def main():
  # filename = "p3data0.txt"
  # filename = "p3data1.txt"
  # filename = "p3data2.txt"
  # filename = "p3data3.txt"
  # filename = "p3data4.txt"
  # filename = "p3data5.txt"
  filename = "p3data6.txt"

  knapsack_start = dt.datetime.now() # 프로그램 시작 시간 기록
  # 파일 읽기
  data = open(filename, "r", encoding="UTF-8") # 데이터 파일 열기
  data_knap = data.read().splitlines()

  N = int(data_knap[0]) # 물건 갯수
  M = int(data_knap[3]) # 무게 제한
  P = [int(i) for i in data_knap[1].split(' ')] # 무게별 이익
  W = [int(i) for i in data_knap[2].split(' ')] # 무게별 무게

  # 단위 무게당 이익; 삽입 정렬을 이용해 정렬
  p_N_w = [[p, w, round(float(p / w), 3), i] \
                      for p, w, i in zip(P, W, range(N))]
  p_N_w = insert_sort(p_N_w)
  del P
  del W
  data.close() # 파일 닫기

  # 노드 생성 (현재 이익, 현재 무게, 최대 이익, depth, 부모 노드)
  root = tn.tree_node(0, 0, bound(p_N_w, M, 0), 0, None, -1)
  # Best search를 통해 답 반환
  max_node = knapsack_best_search(p_N_w, M, root)

  '''
  # 만들어진 트리를 출력하기 원한다면
  # print_node(N, root, 0)
  '''

  # 입력을 포함한 결과 출력하기
  print_answer(N, M, p_N_w, max_node)
  knapsack_elapsed = dt.datetime.now() - knapsack_start # 수행 시간 기록
  print()
  print("Execution time =", knapsack_elapsed.microseconds, "microseconds")
  print("(Elapsed datetime.now() :", knapsack_elapsed, ")")

'''
knapsack Best search
p_N_w : [[p(i), w(i), p(i) / w(i), i], ...]
'''
def knapsack_best_search(p_N_w, M, root):
  N = len(p_N_w)
  prior_que = []
  max_node = root  # 현재 이익이 최대인 노드

  # TODO : 우선순위 큐 구현하기
  priority_push(prior_que, root)

  while (prior_que != []):
    node = priority_pop(prior_que) # 기대 이익이 제일 큰 노드 반환
    depth = node.depth # 현재 노드의 깊이 (0에서 시작)

    left_flag = 0 # 만들어진 왼쪽 자식을 트리에 포함시킬지의 여부
    right_flag = 0 # 만들어진 오른쪽 자식을 트리에 포함시킬지의 여부

    if (node.ubd > max_node.profit): # 예상 이익이 크다면
      # 왼쪽 자식 - (해당 인덱스 물건 포함) 체크 
      left_profit = node.profit + p_N_w[depth][0]
      left_weight = node.weight + p_N_w[depth][1]
      add_profit = bound(p_N_w, M - left_weight, depth + 1) # 추가될 기대 이익

      if (left_weight <= M and depth + 1 < N): # 무게 초과거나 끝이 아니라면
        left_bound = left_profit + add_profit
        # 왼쪽 자식노드 생성
        left_child = tn.tree_node(
                  left_profit, left_weight, left_bound, depth + 1, node, 0)

        # 현재 이익 최고값보다 자식의 이익값이 높다면 수정
        if (left_profit > max_node.profit):
          max_node = left_child
          left_flag = 1 # 트리에 포함시킴

        if (left_bound > max_node.profit): # 기대 이익이 최고 이익보다 크다면
          priority_push(prior_que, left_child)
          left_flag = 1 # 트리에 포함시킴

        if (left_flag): 
          node.set_left_child(left_child)

      # 오른쪽 자식 - (해당 인덱스 물건 불포함) 체크
      right_profit = node.profit
      right_weight = node.weight
      add_profit = bound(p_N_w, M - right_weight, depth + 1) # 추가될 기대 이익

      if (right_weight < M and depth + 1 < N): # 무게 초과거나 끝이 아니라면
        right_bound = right_profit + add_profit
        # 오른쪽 자식노드 생성
        right_child = tn.tree_node(
                  right_profit, right_weight, right_bound, depth + 1, node, 1)

        # 현재 이익 최고값보다 자식의 이익값이 높다면 수정
        if (right_profit > max_node.profit):
          max_node = right_child
          right_flag = 1 # 트리에 포함시킴

        if (right_bound > max_node.profit):  # 기대 이익이 최고 이익보다 크다면
          priority_push(prior_que, right_child)
          right_flag = 1 # 트리에 포함시킴

        if (right_flag):
          node.set_right_child(right_child)

    prior_que = priority_sort(prior_que) # 우선순위 정렬하기 

  return max_node  # 결과 노드 반환하기

'''
Pi / Wi 정렬을 위한 삽입 정렬 함수 - 해당값을 내림차순으로 정렬
p_N_w : [[p(i), w(i), p(i) / w(i), i], ...]
'''
def insert_sort(p_N_w):
  for x in range(1, len(p_N_w)): # 두번째 ~ 끝 원소에 대해
    y = x - 1 # 하나 앞의 원소부터 앞쪽으로 진행할 인덱스
    key = p_N_w[x] # 정렬할 x번째 위치의 원소 
    # 인덱스가 조건을 만족하고 키값이 앞쪽에 위치한(y) 값보다 크다면
    while (y > -1 and p_N_w[y][2] < key[2]): 
      p_N_w[y + 1] = p_N_w[y] # y번째 원소의 값을 하나 뒤에도 복사
      y -= 1 # 계속 앞쪽 인덱스에 대해 진행

    p_N_w[y + 1] = key # 탐색이 끝난 위치에 키 값 저장

  return p_N_w

'''
노드 이하의 탐색에 대해 예상되는 추가 이익 상한을 계산하는 함수
p_N_w : [[p(i), w(i), p(i) / w(i), i], ...]
M : 무게 상한
i : p_N_w 에서 탐색을 시작해야 할 부분의 인덱스 
'''
def bound(p_N_w, M, i):
  remain_M = M # 무게 상한
  ind = i
  add_profit = 0 # 현재 이익에 추가될 이익

  while (ind < len(p_N_w) and remain_M - p_N_w[ind][1] >= 0): # 조건을 만족할 때
    add_profit += p_N_w[ind][0] # 이익을 더해주고
    remain_M -= p_N_w[ind][1] # 무게 상한은 빼주고
    ind += 1
  if (ind < len(p_N_w) and remain_M > 0): # 남은 무게에 대해 fractional knapsack
    add_profit += remain_M * p_N_w[ind][2]

  return add_profit

'''
prior_que에 새 노드를 삽입하고 정렬해주는 wrapper
prior_que : 우선순위 큐로 사용할 리스트
key : 정렬할 속성값
'''
def priority_push(prior_que, tr_node):
  prior_que.append(tr_node)

'''
list 자료형을 주어진 키값을 이용해 우선순위 따라 정렬해주는 wrapper
prior_que : 우선순위 큐로 사용할 리스트
'''
def priority_sort(prior_que): # 정렬하는 키는 node.ubd
  prior_que = sorted (prior_que, key=lambda node: node.ubd, reverse=True)
  return prior_que

'''
오름차순으로 나열된 prior_que에서 우선순위가 높은 노드를 반환해주는 wrapper
prior_que : 우선순위 큐로 사용할 리스트
'''
def priority_pop(prior_que):
  node = prior_que[0] # 리스트 제일 앞 값을 기록하고 삭제
  del prior_que[0]

  return node

# pre order로 트리를 출력하는 함수
def print_node(N, node, depth):
  if (depth < N):
    node.print_node()
    if (node.children[0] != None):
      print_node(N, node.get_left_child(), depth + 1)
    if (node.children[1] != None):
      print_node(N, node.get_right_child(), depth + 1)

'''
결과 벡터를 출력하는 함수
p_N_w : [[p(i), w(i), p(i) / w(i), i], ...]
max_node : best-search 결과로 얻어진 최대 이익 노드
'''
def print_answer(N, M, p_N_w, max_node):
  # p_N_w 순서대로 정렬된 트리의 knapsack 벡터 
  tree_result = max_node.print_knapsack_vector(N)

  result = [0 for i in range(N)] # 반환될 결과 벡터
  for i in range(N):
    if (tree_result[i] == 1): # 포함되는 물건이라면
      origin_ind = p_N_w[i][3] # 주어진 물건의 원래 인덱스
      result[origin_ind] = tree_result[i]

  # 조금 나은 출력을 위해
  end_with = ["\n", ' ']
  end = end_with[1]
  if (N > 15):
    end = end_with[0]

  print("N =", N, "\tM =", M)
  print("Pi =", end=end)
  print([pnw[0] for pnw in p_N_w])
  print("Wi =", end=end)
  print([pnw[1] for pnw in p_N_w])
  print("Pi / Wi =", end=end)
  print([pnw[1] for pnw in p_N_w])
  print()
  print("The maximum profit =", max_node.profit)
  print("The solution vector X =", end=end)
  print(result)

if __name__ == "__main__":
  main()
