'''
# 2017-1 Computer Algorithm Programming Report 01 
# 1202046 서나윤  
# python 3.6
# 실행 결과는 파일로 저장합니다.
# eucl : euclidean / recur : recursion / iter : iteration
'''

import datetime

def main():
  # type A 데이터 계산
  euclidean_algorithm("data1.txt", "R")
  euclidean_algorithm("data1.txt", "I")

  # type B 데이터 계산
  # euclidean_algorithm("data2.txt", "R")
  euclidean_algorithm("data2.txt", "I")

'''
# 주어진 입력들의 최대공약수를 구하는 함수
# filename : 데이터를 읽어올 파일 이름
# RorI : R이면 재귀, I면 반복
'''
def euclidean_algorithm(filename, RorI):
  if (RorI == "r" or RorI == "R"):
    print_eucl_recur(filename) # 주어진 데이터에 따라 재귀 실행 결과 출력
  elif (RorI == "i" or RorI == "I"):
    print_eucl_iter(filename) # 주어진 데이터에 따라 반복 실행 결과 출력

'''
# filename : 데이터를 읽어올 파일의 이름
# data : [[u1, v1], [u2, v2], ...]
# elapsed : 연산에 걸린 시간
'''
def print_eucl_recur(filename):
  data = read_input(filename) # 데이터 읽어 들임

  with open("recur_" + filename, "w", encoding="UTF-8") as out: # 파일로 출력
    for d in data: # 각 숫자 쌍에 대해
      time_start = get_timestamp() # 수행 시작 시의 시간
      result = eucl_recur(d[0], d[1]) # 최대공약수 계산
      time_end = get_timestamp() # 수행이 끝났을 때의 시간
      elapsed = time_end - time_start  # 실행에 걸린 총 시간

      # 출력 내용
      out.write("\t\t\t(1) Recursion\t\t\t\n")
      out.write("U = " + str(d[0]) + "\t")
      out.write("V = " + str(d[1]) + "\n")
      out.write("\tgreatest common divisor (U, V) = " + str(result) + "\n")
      out.write("\texecution time = " + str(elapsed.microseconds) + "μs" + "\n")

  out.close()

'''
# filename : 데이터를 읽어올 파일의 이름
# data : [[u1, v1], [u2, v2], ...]
# elapsed : 연산에 걸린 시간
'''
def print_eucl_iter(filename):
  data = read_input(filename) # 데이터 읽어 들임

  with open("iter_" + filename, "w", encoding="UTF-8") as out: # 파일로 출력
    for d in data: # 각 숫자 쌍에 대해
      result, elapsed = eucl_iter(d[0], d[1]) # 최대공약수 계산값, 실행 시간 반환

      # 출력 내용
      out.write("\t\t\t(2) Iteration\t\t\t\n")
      out.write("U = " + str(d[0]) + "\t")
      out.write("V = " + str(d[1]) + "\n")
      out.write("\tgreatest common divisor (U, V) = " + str(result) + "\n")
      out.write("\texecution time = " + str(elapsed.microseconds) + "μs" + "\n")

  out.close()

'''
# filename 파일을 열어 데이터를 읽고 이중 리스트로 반환한다.
# data : [[u1, v1], [u2, v2], ...]
'''
def read_input (filename):
  f = open(filename, "r", encoding="UTF-8") # 파일 열기
  r = f.readline()

  data = []
  for r in f.readlines(): # 각 줄에 대해
    a = r.split(' ')
    data.append([int(a[0]), int(a[1][:-1])])

  f.close()
  return data

'''
# num_a, num_b의 최대공약수를 재귀함수로 계산
'''
def eucl_recur(num_a, num_b):
  U = max(num_a, num_b) # 피제수
  V = min(num_a, num_b) # 제수

  R = U % V # 나머지
  if (R == 0):
    return V # 나머지가 0이면 제수를 반환
  # 제수가 다음 연산의 피제수, 나머지가 다음 연산의 제수
  result = eucl_recur(V, R)
  return result

'''
# num_a, num_b의 최대공약수를 반복함수로 계산
'''
def eucl_iter(num_a, num_b):
  time_start = get_timestamp() # 프로그램 시작 시의 시간
  U = max(num_a, num_b) # 피제수
  V = min(num_a, num_b) # 제수
  R = U % V # 나머지

  while (R != 0): # 나누어 떨어질때까지 계산
    U = V # 제수가 다음 연산의 피제수 
    V = R # 나머지가 다음 연산의 제수
    R = U % V

  time_end = get_timestamp() # 프로그램이 끝났을 때의 시간
  return V, time_end - time_start # 제수와 실행 시간을 반환

# time.time()의 연산 결과를 소수점 이하 5자리의 마이크로초로 계산하기 위한 함수
def get_timestamp ():
  time_stmp = datetime.datetime.now()
  return time_stmp

if __name__ == "__main__":
  main()
