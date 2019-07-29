import React, { useState } from 'react';

const Info = ({ type, children }) =>
	<div>
		<span>{ type }:</span>
		<span>{ children }</span>
	</div>;

export default () => {
	const [ isRunning, setRunning ] = useState(false);
	const startStop = () => {
		setRunning(!isRunning);
	}

	const [ bay, setBay ] = useState('A&B');
	const changeBay = () => {
		setBay(
			bay === 'A&B' ? 'A' :
			bay === 'A'   ? 'B' :
			'A&B'
		);
	}

	const [ faceEdge, setFaceEdge ] = useState(false);
	const changeFaceEdge = () => {
		setFaceEdge(!faceEdge);
	};
	
	return <>
		<div>
			<Info type="Away">7s</Info>
			<Info type="Face">3s</Info>
			<Info type="Rep">5x</Info>
			<Info type="Wifi">On</Info>
			<Info type="Bay">{ bay }</Info>
		</div>

		<button onClick={startStop}>{ isRunning ? 'Stop' : 'Start' }</button>
		{
			!isRunning ? <>
				<button> Setup </button>
				<button onClick={changeBay}> Bay </button>
				<button onClick={changeFaceEdge}>{ faceEdge ? 'Face' : 'Edge' }</button>
			</> : []
		}
	</>;
};
