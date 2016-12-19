

function belt_slash (hab, belt_anime, life_swing)


    base_trun = random()*360
    belt_point_num = 30
    
    belt_anime.drawRange: addCascade(D3DXVECTOR2(1,1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(belt_point_num-1,belt_point_num-1),life_swing)


    for k = 0, belt_point_num do
        turn_matrix = D3DXMATRIX()

        pos = PositionPerGrid(0,0,1)

        D3DXMatrixRotationX(turn_matrix, ((k/belt_point_num)*300-75)*MATH_PI/180 )
        D3DXVec3TransformCoord(pos,pos,turn_matrix)

        D3DXMatrixRotationY(turn_matrix, 30*MATH_PI/(180) )
        D3DXVec3TransformCoord(pos,pos,turn_matrix)

        D3DXMatrixRotationZ(turn_matrix, 5*MATH_PI/(180) )
        D3DXVec3TransformCoord(pos,pos,turn_matrix)
        pos.x = pos.x*0.5

        belt_anime.rightEdge: push_back(VariationValue_PositionPerGrid(pos*0.4))
        belt_anime.leftEdge: push_back(VariationValue_PositionPerGrid(pos))

    end
    belt_anime2 = AnimeChip_Belt(belt_anime);
    belt_anime2: setLeftEdgeStartingPositionalPoint(PositionPerGrid(0.0, -0.35, 0.5))
    belt_anime2: setRightEdgeStartingPositionalPoint(PositionPerGrid(0.05, 0, 0))

    belt_anime3 = AnimeChip_Belt(belt_anime);
    belt_anime3.drawMode: setDefault(DrawMode_addition)
    belt_anime3: setDelay(belt_anime: getDelay()+2)
    belt_anime4 = AnimeChip_Belt(belt_anime2);
    belt_anime4.drawMode: setDefault(DrawMode_addition)
    belt_anime4: setDelay(belt_anime: getDelay()+2)

    hab: addAnimeChip(belt_anime);
    hab: addAnimeChip(belt_anime2);
    hab: addAnimeChip(belt_anime3);
    hab: addAnimeChip(belt_anime4);
end