(* ::Package:: *)

(*
  # LICENSE
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

  Copyright 2023, Furkan Semih D\[CapitalUDoubleDot]NDAR
  Email: f.semih.dundar@yandex.com
*)


BeginPackage["Variety`"];


<<SetReplace`


UniqueVertices[hg_]:=hg//Flatten//Union;


NumOfUniqueVertices[hg_]:=hg//UniqueVertices//Length;


NeighborOfHyperedge[he_,hg_]:=If[IntersectingQ[he,#],#,Nothing]&/@hg;


DeleteHyperedge[he_,hg_]:=If[he!=#,#,Nothing]&/@hg;


TreeOfHyperedge[he_,hg_]:=If[hg=={},
Tree[he,{}],
Tree[he,TreeOfHyperedge[#,DeleteHyperedge[he,hg]]&/@NeighborOfHyperedge[he,DeleteHyperedge[he,hg]]]
];


TreeOfHypergraph[hg_]:=TreeOfHypergraph[hg]=Tree[{},Map[TreeOfHyperedge[{#},hg]&,UniqueVertices[hg]]];


TreeOfVertex[u_,tr_]:=TreeOfVertex[u,tr]=First[If[TreeData[#]=={u},#,Nothing]&/@TreeChildren[tr]];


NeighborOfVertexAtDepth[u_,tr_,d_]:=NeighborOfVertexAtDepth[u,tr,d]=TreeData[#]&/@TreeLevel[TreeOfVertex[u,tr],d];


RelativeIndifference[u_,v_,hg_,tr_]:=RelativeIndifference[Min[u,v],Max[u,v],hg,tr]=Module[
{ut=TreeOfVertex[u,tr],vt=TreeOfVertex[v,tr]},
For[d=1,d<=Min[TreeDepth[ut],TreeDepth[vt]],d++,
If[Not[IsomorphicHypergraphQ[NeighborOfVertexAtDepth[u,tr,d],NeighborOfVertexAtDepth[v,tr,d]]],
Return[d],
Continue[]]];
Return[0];
];


AbsoluteIndifference[u_,hg_,tr_]:=Module[
{ris=RelativeIndifference[u,#,hg,tr]&/@DeleteElements[UniqueVertices[hg],{u}]},
If[MemberQ[ris,0],
0,
Max[ris]
]
]


Variety[hg_]:=Variety[hg]=Module[
{ais=AbsoluteIndifference[#,hg,TreeOfHypergraph[hg]]&/@UniqueVertices[hg]},
If[MemberQ[ais,0],
0,
Fold[Plus,1/#&/@ais]]
]


ParallelVariety[hg_]:=ParallelVariety[hg]=Module[
{ais=ParallelMap[AbsoluteIndifference[#,hg,TreeOfHypergraph[hg]]&,UniqueVertices[hg]]},
If[MemberQ[ais,0],
0,
Fold[Plus,1/#&/@ais]]
]


DynamicVariety[hg_]:=DynamicVariety[hg]= If[NumOfUniqueVertices[hg]>$KernelCount,
ParallelVariety[hg],
Variety[hg]]


VarietyFromAis[xs_,f_:(1/#&)]:=If[MemberQ[xs,0],0,Total[f/@xs]];


ActionOfPath[path_]:=Module[
{vars=DynamicVariety/@VertexList[path]},
If[LeibnizianPathQ[path],
Total[vars],
0]
]


ActionOfPathFromAis[path_,varList_]:=Module[
{vars=VertexList[path]/.varList},
If[MemberQ[vars,0],
0,
Total[vars]]
]


LeibnizianPathQ[path_]:=Not[MemberQ[DynamicVariety/@VertexList[path],0]]


Begin["Private`"];


End[];


EndPackage[];
