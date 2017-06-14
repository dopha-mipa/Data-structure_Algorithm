# 1202046 서나윤 컴퓨터 알고리즘 2017-2 Written Report 01
# The Celebrity problem - 리스트를 이용한 방식
import numpy as np

def famous (party) :
  num = len(party)
  part_list = [0] * num

  for i in range (num) :
    part_list[i] = i
  
  A = 0 # 첫 사람 
  B = num - 1 # 끝 사람

  # 유명인의 조건 확인
  while (A < B) :
    if (party[A][B] == 1) : # A가 B를 안다면 A는 유명인이 아니다
      A += 1
    else : # A가 B를 모르면 B는 유명인이 아니다 
      B -= 1

  # A (혹은 B)가 유명인인지 확인한다
  for i in range (num) :
    if (party[i][A] != 1 and i != A) :
      print (str(A) + " is not celebrity.")
      return;

  print (str(A) + " is celebrity.")


if __name__ == "__main__" :
  '''
  # 사람의 수 : 6명 (0 ~ 5)
  # 유명인 : 5번
  '''
  party = [[0, 0, 0, 1, 0, 1],
           [0, 0, 1, 0, 1, 1],
           [0, 1, 0, 1, 0, 1],
           [1, 0, 0, 0, 0, 1],
           [1, 0, 0, 1, 0, 1],
           [0, 0, 0, 0, 0, 0]]
  famous (party)
