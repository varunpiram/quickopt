"use strict";(self.webpackChunkdocs=self.webpackChunkdocs||[]).push([[201],{5031:(e,n,s)=>{s.r(n),s.d(n,{assets:()=>c,contentTitle:()=>o,default:()=>h,frontMatter:()=>r,metadata:()=>l,toc:()=>d});var i=s(4848),t=s(8453);const r={sidebar_position:2},o="Usage",l={id:"pso/usage",title:"Usage",description:"pso",source:"@site/docs/pso/usage.md",sourceDirName:"pso",slug:"/pso/usage",permalink:"/quickopt/docs/pso/usage",draft:!1,unlisted:!1,tags:[],version:"current",sidebarPosition:2,frontMatter:{sidebar_position:2},sidebar:"tutorialSidebar",previous:{title:"Description",permalink:"/quickopt/docs/pso/description"},next:{title:"Examples",permalink:"/quickopt/docs/pso/examples"}},c={},d=[{value:"<code>pso</code>",id:"pso",level:2},{value:"Signature",id:"signature",level:3},{value:"Parameters",id:"parameters",level:3},{value:"Output",id:"output",level:3},{value:"Notes",id:"notes",level:3}];function a(e){const n={code:"code",em:"em",h1:"h1",h2:"h2",h3:"h3",li:"li",p:"p",pre:"pre",strong:"strong",ul:"ul",...(0,t.R)(),...e.components};return(0,i.jsxs)(i.Fragment,{children:[(0,i.jsx)(n.h1,{id:"usage",children:"Usage"}),"\n",(0,i.jsx)(n.h2,{id:"pso",children:(0,i.jsx)(n.code,{children:"pso"})}),"\n",(0,i.jsxs)(n.p,{children:["Uses particle swarm optimization to ",(0,i.jsx)(n.strong,{children:"maximize"})," a function of double inputs."]}),"\n",(0,i.jsx)(n.h3,{id:"signature",children:"Signature"}),"\n",(0,i.jsx)(n.pre,{children:(0,i.jsx)(n.code,{className:"language-python",children:"pso(funct, space_min, space_max, iterations=100, swarm_size=100, inertia=0.5, cognitive=1.0, social=2.0, clamp=0.1, verbose=1)\n"})}),"\n",(0,i.jsx)(n.h3,{id:"parameters",children:"Parameters"}),"\n",(0,i.jsxs)(n.ul,{children:["\n",(0,i.jsxs)(n.li,{children:["\n",(0,i.jsxs)(n.p,{children:[(0,i.jsx)(n.code,{children:"funct"})," - ",(0,i.jsx)(n.strong,{children:"function: (input: List[float]) -> float"})," : The function to be maximized. It should take a list of doubles as input and return a double."]}),"\n"]}),"\n",(0,i.jsxs)(n.li,{children:["\n",(0,i.jsxs)(n.p,{children:[(0,i.jsx)(n.code,{children:"space_min"})," - ",(0,i.jsx)(n.strong,{children:"List[float]"})," : Lower bound vector for the search space. Defines the minimum values for the parameters."]}),"\n"]}),"\n",(0,i.jsxs)(n.li,{children:["\n",(0,i.jsxs)(n.p,{children:[(0,i.jsx)(n.code,{children:"space_max"})," - ",(0,i.jsx)(n.strong,{children:"List[float]"})," : Upper bound vector for the search space. Defines the maximum values for the parameters."]}),"\n"]}),"\n",(0,i.jsxs)(n.li,{children:["\n",(0,i.jsxs)(n.p,{children:[(0,i.jsx)(n.code,{children:"iterations"})," - ",(0,i.jsx)(n.strong,{children:"int"}),", ",(0,i.jsx)(n.em,{children:(0,i.jsx)(n.strong,{children:"optional"})})," : The number of iterations to run the algorithm. Default is 100."]}),"\n"]}),"\n",(0,i.jsxs)(n.li,{children:["\n",(0,i.jsxs)(n.p,{children:[(0,i.jsx)(n.code,{children:"swarm_size"})," - ",(0,i.jsx)(n.strong,{children:"int"}),", ",(0,i.jsx)(n.em,{children:(0,i.jsx)(n.strong,{children:"optional"})})," : The number of particles in the swarm. Default is 100."]}),"\n"]}),"\n",(0,i.jsxs)(n.li,{children:["\n",(0,i.jsxs)(n.p,{children:[(0,i.jsx)(n.code,{children:"inertia"})," - ",(0,i.jsx)(n.strong,{children:"double"}),", ",(0,i.jsx)(n.em,{children:(0,i.jsx)(n.strong,{children:"optional"})})," : Inertia weight. Recommended to be in (0,1). Default is 0.5."]}),"\n"]}),"\n",(0,i.jsxs)(n.li,{children:["\n",(0,i.jsxs)(n.p,{children:[(0,i.jsx)(n.code,{children:"cognitive"})," - ",(0,i.jsx)(n.strong,{children:"double"}),", ",(0,i.jsx)(n.em,{children:(0,i.jsx)(n.strong,{children:"optional"})})," : Cognitive weight. Recommended to be in [1,3]. Default is 1.0."]}),"\n"]}),"\n",(0,i.jsxs)(n.li,{children:["\n",(0,i.jsxs)(n.p,{children:[(0,i.jsx)(n.code,{children:"social"})," - ",(0,i.jsx)(n.strong,{children:"double"}),", ",(0,i.jsx)(n.em,{children:(0,i.jsx)(n.strong,{children:"optional"})})," : Social weight. Recommended to be in [1,3]. Default is 2.0."]}),"\n"]}),"\n",(0,i.jsxs)(n.li,{children:["\n",(0,i.jsxs)(n.p,{children:[(0,i.jsx)(n.code,{children:"clamp"})," - ",(0,i.jsx)(n.strong,{children:"double"}),", ",(0,i.jsx)(n.em,{children:(0,i.jsx)(n.strong,{children:"optional"})})," : Clamping limit on velocity. Default is 0.1."]}),"\n"]}),"\n",(0,i.jsxs)(n.li,{children:["\n",(0,i.jsxs)(n.p,{children:[(0,i.jsx)(n.code,{children:"verbose"})," - ",(0,i.jsx)(n.strong,{children:"int"}),", ",(0,i.jsx)(n.em,{children:(0,i.jsx)(n.strong,{children:"optional"})})," : The verbosity level. ",(0,i.jsx)(n.code,{children:"0"})," for final output, ",(0,i.jsx)(n.code,{children:"1"})," for output at each iteration. Default is 1."]}),"\n"]}),"\n"]}),"\n",(0,i.jsx)(n.h3,{id:"output",children:"Output"}),"\n",(0,i.jsxs)(n.ul,{children:["\n",(0,i.jsxs)(n.li,{children:[(0,i.jsx)(n.code,{children:"best_params"})," - ",(0,i.jsx)(n.strong,{children:"List[float]"}),": The best set of parameters found by the particle swarm optimization process."]}),"\n"]}),"\n",(0,i.jsx)(n.h3,{id:"notes",children:"Notes"}),"\n",(0,i.jsxs)(n.ul,{children:["\n",(0,i.jsx)(n.li,{children:"Minimization can be achieved by returning the negative of objective function values."}),"\n",(0,i.jsxs)(n.li,{children:["The ",(0,i.jsx)(n.code,{children:"space_min"})," and ",(0,i.jsx)(n.code,{children:"space_max"})," vectors must be of the same size."]}),"\n",(0,i.jsxs)(n.li,{children:["Ensure that the ",(0,i.jsx)(n.code,{children:"inertia"})," is in (0,1), and the ",(0,i.jsx)(n.code,{children:"cognitive"})," and ",(0,i.jsx)(n.code,{children:"social"})," weights are in [1,3]."]}),"\n",(0,i.jsxs)(n.li,{children:["Adjust the verbosity level with the ",(0,i.jsx)(n.code,{children:"verbose"})," parameter to control the amount of output during the iterations."]}),"\n"]})]})}function h(e={}){const{wrapper:n}={...(0,t.R)(),...e.components};return n?(0,i.jsx)(n,{...e,children:(0,i.jsx)(a,{...e})}):a(e)}},8453:(e,n,s)=>{s.d(n,{R:()=>o,x:()=>l});var i=s(6540);const t={},r=i.createContext(t);function o(e){const n=i.useContext(r);return i.useMemo((function(){return"function"==typeof e?e(n):{...n,...e}}),[n,e])}function l(e){let n;return n=e.disableParentContext?"function"==typeof e.components?e.components(t):e.components||t:o(e.components),i.createElement(r.Provider,{value:n},e.children)}}}]);