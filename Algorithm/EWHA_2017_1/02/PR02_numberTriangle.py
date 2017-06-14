'''
# PR02 1202046 서나윤 - 2017. 05. 09. 컴퓨터 알고리즘
# Maximum summation of number triangle
# python 3.5, Sublime Text로 작성
'''

# main 함수
def main():
  # 데이터 입력부
  triangle = readFile("data0.txt")
  # triangle = readFile("data1.txt")
  # triangle = readFile("data2.txt")
  # triangle = readFile("data3.txt")
  # triangle = readFile("data4.txt")
  # triangle = readFile("data5.txt")

  # 함수 실행부
  # '''
  path, total = nt_forward(triangle) # 위->아래 탐색
  print_result("Forward", path, total) # 결과 출력
  print()
  path, total = nt_backward(triangle) # 아래->위 탐색
  print_result("Backward", path, total) # 결과 출력
  # '''

'''
# 삼각형의 위쪽부터 아래 방향으로 가능한 노선을 탐색하며
# 탐색 위치에 큰 합을 만드는 노선과 합을 기록
# triangle : 파일의 데이터로 만든 (행, 열) 이중 리스트
'''
def nt_forward(triangle):
  height = len(triangle) # 삼각형의 높이

  srch_tri = init_forward(triangle) # 양 끝의 결과값이 입력된 (행, 열) 리스트 반환

  # 양끝을 제외하고 남은 가운데 값들에 대해서 (셋째 줄 ~, 둘째 열 ~ (줄 - 1) 열)
  for row in range(2, height): 
    for col in range(1, row):
      # 가능한 이전 노선 중 큰 값
      max_val = max(srch_tri[row - 1][col - 1][1], srch_tri[row - 1][col][1])
      sum_m = triangle[row][col] + max_val # 노선에 따른 합

      if (max_val == srch_tri[row - 1][col - 1][1]): # 왼쪽 부모가 크다면
        srch_tri[row][col] = [(row - 1, col - 1), sum_m] 
      else : # 오른쪽 부모가 크다면
        srch_tri[row][col] = [(row - 1, col), sum_m]

  # 삼각형 제일 아랫줄의 원소들에 대해서 최대값 찾기
  max_ind = 0
  for i in range(height):
    if (srch_tri[height - 1][max_ind][1] < srch_tri[height - 1][i][1]):
      max_ind = i

  path = [] # 경로를 기록
  path.insert(0, [(height - 1, max_ind), triangle[height - 1][max_ind]])
  coord = srch_tri[height - 1][max_ind][0] # 노선의 이전 좌표. coordination
  while ((row, col) != (0, 0)):
    row, col = coord[0], coord[1] # 삼각형에서의 위치 
    path.insert(0, [coord, triangle[row][col]])
    coord = srch_tri[row][col][0] # 다음으로 진행

  return path, srch_tri[height - 1][max_ind][1] # 경로, 합 반환

'''
# triangle : 파일의 데이터로 만든 (행, 열) 이중 리스트
# srch_tri : search_triangle. [(이전 좌표), 노선의 합] (행, 열) 삼중 리스트
# srch_tri를 생성한 후 왼쪽, 오른쪽 끝으로만 이동하는 노선을 기록하고 반환
'''
def init_forward(triangle):
  srch_tri = []
  height = len(triangle)
  # (행, 열)로 인덱싱한 리스트 반환
  for i in range(height):
    srch_tri.append([[(0, 0), 0] for j in range(i + 1)])

  srch_tri[0][0] = [(0, 0), triangle[0][0]] # (0, 0) 위치 초기화 

  for row in range(1, height):
    # 왼쪽 끝 초기화
    srch_tri[row][0] = \
        [(row - 1, 0), triangle[row][0] + srch_tri[row - 1][0][1]]
    # 오른쪽 끝 초기화
    srch_tri[row][row] = \
        [(row - 1, row - 1), \
         triangle[row][row] + srch_tri[row - 1][row - 1][1]]

  return srch_tri

'''
# 삼각형의 아래쪽부터 위 방향으로 가능한 노선을 탐색하며
# 탐색 위치에 큰 합을 만드는 노선과 합을 기록
'''
def nt_backward(triangle):
  height = len(triangle)
  srch_tri = init_backward(triangle)

  for row in range(height - 2, -1, -1): # 아래에서 하나 윗줄부터 -> 0번 줄로
    for col in range(row + 1):
      # 가능한 이전 노선 중 큰 값
      max_val = max(srch_tri[row + 1][col][1], srch_tri[row + 1][col + 1][1])
      summ = triangle[row][col] + max_val

      if (max_val == srch_tri[row + 1][col][1]): # 왼쪽 자식이 크다면
        srch_tri[row][col] = [(row + 1, col), summ]
      else: # 오른쪽 자식이 크다면
        srch_tri[row][col] = [(row + 1, col + 1), summ]

  path = [] # 경로를 기록할 리스트
  path.append([(0, 0), triangle[0][0]]) # 시작점 추가
  coord = srch_tri[0][0][0] # 다음 점의 좌표
  for _ in range(height - 1):
    row, col = coord[0], coord[1]
    path.append([(row, col), triangle[row][col]])
    coord = srch_tri[row][col][0]

  return path, srch_tri[0][0][1] # 경로와 합을 반환

'''
# triangle : 파일의 데이터로 만든 (행, 열) 이중 리스트
# srch_tri : [(이전 좌표), 노선의 합] (행, 열) 삼중 리스트
# srch_tri를 생성하고 반환
'''
def init_backward(triangle):
  srch_tri = []
  height = len(triangle)
  # (행, 열)로 인덱싱한 삼중 리스트 반환
  for i in range(height):
    srch_tri.append([[(0, 0), triangle[i][j]] for j in range(i + 1)])

  return srch_tri

'''
# 파일을 읽어 이중리스트 삼각형으로 반환
'''
def readFile(filename):
  triangle = []

  with open(filename, "r", encoding="UTF-8") as f:
    f.readline()
    for line in f.readlines():
      l = line.strip().split(' ')
      l = [int(x) for x in l]
      triangle.append(l)

  return triangle
'''
# nt_forward, nt_backward가 반환한 값을 양식에 맞게 출력
# method = Forward / Backward
# path, total : 알고리즘의 결과 값
'''
def print_result(method, path, total):
  print("\t\t\t\t\t\t\t\t", method)
  print("The maximum sum of path :", total) # 경로의 합 출력
  print("path :") # 경로 출력
  for i in range(len(path)):
    if (i % 5 == 0 and i != 0):
      print("")
    # 좌표 없이 보시려면 아랫줄 주석처리해주세요!
    # print(path[i][0], end=' ')
    print(path[i][1], end=' ')
    if (i != len(path) - 1):
      print("=> ", end = '')
  print()

if __name__ == "__main__":
  main()
